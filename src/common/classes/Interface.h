/*
 *	PROGRAM:		Firebird interface.
 *	MODULE:			Interface.h
 *	DESCRIPTION:	Base class for all FB interfaces / plugins.
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

#ifndef FB_COMMON_CLASSES_INTERFACE
#define FB_COMMON_CLASSES_INTERFACE

//#include "../jrd/gds_proto.h"
#include "../common/classes/alloc.h"

namespace Firebird {

class Interface
{
public:
	//virtual InterfaceBase* queryInterface() = 0;
	//virtual void addRef() = 0;
	virtual void release() = 0;
protected:
	~Interface() { }
};

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
/*
	void* ptr = gds__alloc(sizeof(T));
	return new(ptr) T;
 */
 	return FB_NEW(*getDefaultMemoryPool()) T;
}

template <typename T>
void interfaceFree(T* ptr)
{
/*
	delete((void*)0) ptr;
	gds__free(ptr);
 */
 	delete ptr;
}

} // namespace Auth


#endif // FB_COMMON_CLASSES_INTERFACE
