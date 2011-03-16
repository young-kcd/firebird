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

#ifndef FB_INCLUDE_INTERFACE
#define FB_INCLUDE_INTERFACE

#include "types_pub.h"

#ifdef __GNUC__
#	pragma GCC system_header	// disable warning about non-existent virtual destructor
#endif

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#	define FB_CARG __cdecl
#else
#	define FB_CARG
#endif

namespace Firebird {

// Regular interface - base for all FB interfaces
class Interface
{
public:
	virtual void FB_CARG addRef() = 0;
	virtual int FB_CARG release() = 0;
	virtual int FB_CARG version() = 0;
};
#define FB_INTERFACE_VERSION 3		// If this is changed, types of all interfaces must be changed

// Interface to work with status vector
class Status : public Interface
{
public:
	virtual void FB_CARG set(unsigned int length, const ISC_STATUS* value) = 0;
	virtual void FB_CARG set(const ISC_STATUS* value) = 0;
	virtual void FB_CARG init() = 0;

	virtual const ISC_STATUS* FB_CARG get() const = 0;
	virtual int FB_CARG isSuccess() const = 0;
};
#define FB_STATUS_VERSION (FB_INTERFACE_VERSION + 5)

class IPlugin;
class ITimerControl;

class IMaster : public Interface
{
public:
	// This interface can't be upgraded - therefore another form of version is used
	const static unsigned int VERSION = 1;	// To be changed each time any interface is changed

	virtual Status* FB_CARG getStatusInstance() = 0;
	virtual IPlugin* FB_CARG getPluginInterface() = 0;
	virtual int FB_CARG upgradeInterface(Interface* toUpgrade, int desiredVersion, void* missingFunctionClass) = 0;
	virtual const char* FB_CARG circularAlloc(const char* s, size_t len, intptr_t thr) = 0;
	virtual ITimerControl* FB_CARG getTimerControl() = 0;
};

} // namespace Firebird

extern "C"
{
	// additional API function
	Firebird::IMaster* ISC_EXPORT fb_get_master_interface();
}

#endif // FB_INCLUDE_INTERFACE
