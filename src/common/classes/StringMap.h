/*
 *	PROGRAM:	Client/Server Common Code
 *	MODULE:		StringMap.h
 *	DESCRIPTION:	Secure handling of clumplet buffers
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
 *  The Original Code was created by Nickolay Samofatov
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2004 Nickolay Samofatov <nickolay@broadviewsoftware.com>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 *
 *
 *  $Id: StringMap.h,v 1.1 2004-11-22 20:21:54 skidder Exp $
 *
 */

#ifndef STRINGMAP_H
#define STRINGMAP_H

#include "fb_string.h"
#include "../common/classes/tree.h"

namespace Firebird {

class StringMap : public AutoStorage {
public:
	StringMap();
	StringMap(MemoryPool& pool);
	~StringMap();

	// Returns true if value existed previously
	bool put(const string& name, const string& value);

	// Returns true if value existed
	bool remove(const string& name);

	// Returns true if value is found
	bool get(const string& name, string& value);

	size_t count() { return mCount; }

	struct StringPair {
		// We use string class here because we want to handle possible embedded
		// zeros properly
		const string* cvar_name;
		string* cvar_value;

		static const string* const & generate(const void* sender, const StringPair& item) {
			return item.cvar_name;
		}

		static bool greaterThan(const string* const &i1, const string* const &i2) {
			return *i1 > *i2;
		}
	};

	typedef BePlusTree<StringPair, const string*, MemoryPool, StringPair, StringPair> ValuesTree;
private:
	ValuesTree tree;
	size_t mCount;
};

}

#endif