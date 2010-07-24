/*
 *	PROGRAM:		Firebird interface.
 *	MODULE:			ImplementHelper.h
 *	DESCRIPTION:	Tools to help write plugins.
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
 *  The Original Code was created by Alex Peshkov
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2010 Alex Peshkov <peshkoff at mail.ru>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 *
 *
 */

#ifndef FB_COMMON_CLASSES_IMPLEMENT_HELPER
#define FB_COMMON_CLASSES_IMPLEMENT_HELPER

#include "../common/classes/Interface.h"

//#include "../jrd/gds_proto.h"
#include "../common/classes/alloc.h"

namespace Firebird {

// If you need interface on stack, use it in template AutoPtr<YourInterface, AutoInterface>
// as second parameter
class AutoInterface
{
public:
	static void clear(Interface* ptr)
	{
		if (ptr)
		{
			ptr->release();
		}
	}
};

template <typename T>
T* interfaceAlloc()
{
	/***
	void* ptr = gds__alloc(sizeof(T));
	return new(ptr) T;
	***/
 	return FB_NEW(*getDefaultMemoryPool()) T;
}

template <typename T>
void interfaceFree(T* ptr)
{
	/***
	delete((void*) 0) ptr;
	gds__free(ptr);
	***/
 	delete ptr;
}

// Template to create static instance of plugin
template <typename C, unsigned int T, char NM[]>
class PluginHelper : public C
{
public:
	PluginHelper()
	{
		fb_register_plugin(this);
	}

	virtual const char* name() const
	{
		return NM;
	}

	virtual unsigned int type() const
	{
		return T;
	}

	virtual void link(Plugin* chain)
	{
		pp = chain;
	}

	virtual Plugin* next() const
	{
		return pp;
	}

private:
	Plugin* pp;
};

} // namespace Firebird


#endif // FB_COMMON_CLASSES_IMPLEMENT_HELPER
