/*
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
 *  The Original Code was created by Claudio Valderrama on 25-Dec-2003
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2003 Claudio Valderrama
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 *
 */


// =====================================
// Utility functions

#ifndef INCLUDE_UTILS_PROTO_H
#define INCLUDE_UTILS_PROTO_H

#include "../common/classes/fb_string.h"

namespace fb_utils
{
	char* exact_name(char* const str);
	inline void exact_name(Firebird::string& str)
	{
		str.rtrim();
	}
	char* exact_name_limit(char* const str, size_t bufsize);
	int name_length(const TEXT* const name);
	int snprintf(char* buffer, size_t count, const char* format...);
} // namespace fb_utils

#endif // INCLUDE_UTILS_PROTO_H

