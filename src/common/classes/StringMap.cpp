#include "firebird.h"
#include "../common/classes/StringMap.h"

namespace Firebird {

StringMap::StringMap() : tree(&getPool()), mCount(0) {
}

StringMap::StringMap(MemoryPool& pool) : AutoStorage(pool), tree(&getPool()), mCount(0) {
}

StringMap::~StringMap() {
	if (tree.getFirst()) {
		while (true) {
			StringPair temp = tree.current();
			bool haveMore = tree.fastRemove();
			delete temp.cvar_name;
			delete temp.cvar_value;
			if (!haveMore) break;
		}
	}
}

bool StringMap::remove(const string& name) {
	if (tree.locate(&name)) {
		StringPair var = tree.current();
		tree.fastRemove();
		delete var.cvar_name;
		delete var.cvar_value;
		mCount--;
		return true;
	}

	return false;
}

bool StringMap::put(const string& name, const string& value) {
	if (tree.locate(&name)) {
		*tree.current().cvar_value = value;
		return true;
	}

	StringPair var;
	var.cvar_name = FB_NEW(getPool()) string(getPool(), name);
	var.cvar_value = FB_NEW(getPool()) string(getPool(), value);
	tree.add(var);
	mCount++;
	return false;
}

bool StringMap::get(const string& name, string& value) {
	if (tree.locate(&name)) {
		value = *tree.current().cvar_value;
		return true;
	}
	return false;
}


} // namespace Firebird
