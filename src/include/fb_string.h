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
				(string(*this) < string(r)) : 
				(fb_stricmp(c_str(), r.c_str()) < 0);
		}
		inline bool operator<=(const PathName &r) const {
			return CASE_SENSITIVITY ?
				(string(*this) <= string(r)) : 
				(fb_stricmp(c_str(), r.c_str()) <= 0);
		}
		inline bool operator==(const PathName &r) const {
			return CASE_SENSITIVITY ?
				(string(*this) == string(r)) : 
				(fb_stricmp(this->c_str(), r.c_str()) == 0);
		}
		inline bool operator>=(const PathName &r) const {
			return CASE_SENSITIVITY ?
				(string(*this) >= string(r)) : 
				(fb_stricmp(this->c_str(), r.c_str()) >= 0);
		}
		inline bool operator>(const PathName &r) const {
			return CASE_SENSITIVITY ?
				(string(*this) > string(r)) : 
				(fb_stricmp(this->c_str(), r.c_str()) > 0);
		}
		inline bool operator!=(const PathName &r) const {
			return CASE_SENSITIVITY ?
				(string(*this) != string(r)) : 
				(fb_stricmp(this->c_str(), r.c_str()) != 0);
		}
	};
}


#endif	// FB_STRING_H
