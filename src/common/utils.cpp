/*
 *  The contents of this file are subject to the Mozilla Public
 *  License Version 1.1 (the "License"); you may not use this file
 *  except in compliance with the License. You may obtain a copy of
 *  the License at http://www.mozilla.org/MPL/
 *  Alternatively, the contents of this file may be used under the
 *  terms of the GNU General Public License Version 2 or later (the
 *  "GPL"), in which case the provisions of the GPL are applicable
 *  instead of those above. You may obtain a copy of the Licence at
 *  http://www.gnu.org/copyleft/gpl.html
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Relevant for more details.
 *
 *  This file was created by members of the firebird development team.
 *  All individual contributions remain the Copyright (C) of those
 *  individuals.  Contributors to this file are either listed here or
 *  can be obtained from a CVS history command.
 *
 *  All rights reserved.
 *  Claudio Valderrama created this file on 25-Dec-2003.
 *
 *  Contributor(s):
 *
 */


// =====================================
// Utility functions


#if defined(LINUX) || defined(MINGW) || defined(FREEBSD)
#define __need_size_t
#include <stddef.h>
#undef __need_size_t
#endif

namespace fb_utils {

char* fb_exact_name(char* const str)
{
/**************************************
 *
 *	f b _ e x a c t _ n a m e
 *
 **************************************
 *
 * Functional description
 *	Trim off trailing spaces from a metadata name.
 *	eg: insert a null after the last non-blank character.
 *
 *	SQL delimited identifier may have blank as part of the name
 *
 *	Parameters:  str - the string to terminate
 *	Returns:     str
 *
 **************************************/
	char* p = str;
	while (*p)
	    ++p;
	// Now, let's go back
	--p;
	while (p >= str && *p == '\x20') // blank character, ASCII(32)
		--p;
	*(p + 1) = '\0';
	return str;
}

char* fb_exact_name_limit(char* const str, size_t bufsize)
{
/**************************************
 *
 *	f b _ e x a c t _ n a m e _ l i m i t
 *
 **************************************
 *
 * Functional description
 *	Trim off trailing spaces from a metadata name.
 *	eg: insert a null after the last non-blank character.
 *	It has a maximum length to ensure working between bounds.
 *
 *	SQL delimited identifier may have blank as part of the name
 *
 *	Parameters:  str - the string to terminate
 *	Returns:     str
 *
 **************************************/
	const char* const end = str + bufsize - 1;
	char* p = str;
	while (*p && p < end)
	    ++p;
	// Now, let's go back
	--p;
	while (p >= str && *p == '\x20') // blank character, ASCII(32)
		--p;
	*(p + 1) = '\0';
	return str;
}

} // namespace fb_utils


