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
#include "../common/classes/alloc.h"
#include "../include/gen/autoconfig.h"

#include <string>

#ifdef HAVE_STRCASECMP
#define STRCASECMP strcasecmp
#else
#ifdef HAVE_STRICMP
#define STRCASECMP stricmp
#else
#error dont know how to compare strings case insensitive on this system
#endif /* HAVE_STRICMP */
#endif /* HAVE_STRCASECMP */

namespace Firebird
{
	typedef std::basic_string<char, std::char_traits<char>,
				Firebird::allocator<char> > string;

	class PathName : public string {
	public:
		inline PathName(const string &s) : string(s) {}
		inline PathName(const char *s) : string(s) {}
		inline PathName(void) : string() {}
		inline bool operator<(const PathName &r) const {
			return CASE_SENSITIVITY ?
			  (*this < r) : (STRCASECMP(this->c_str(), r.c_str()) < 0);
		}
		inline bool operator<=(const PathName &r) const {
			return CASE_SENSITIVITY ?
			  (*this <= r) : (STRCASECMP(this->c_str(), r.c_str()) <= 0);
		}
		inline bool operator==(const PathName &r) const {
			return CASE_SENSITIVITY ?
			  (*this == r) : (STRCASECMP(this->c_str(), r.c_str()) == 0);
		}
		inline bool operator>=(const PathName &r) const {
			return CASE_SENSITIVITY ?
			  (*this >= r) : (STRCASECMP(this->c_str(), r.c_str()) >= 0);
		}
		inline bool operator>(const PathName &r) const {
			return CASE_SENSITIVITY ?
			  (*this > r) : (STRCASECMP(this->c_str(), r.c_str()) > 0);
		}
		inline bool operator!=(const PathName &r) const {
			return CASE_SENSITIVITY ?
			  (*this != r) : (STRCASECMP(this->c_str(), r.c_str()) != 0);
		}
	};
}


#endif	// FB_STRING_H
