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

namespace Firebird {

// Regular interface, typically returned by plugin or other interface
class Interface
{
public:
	//virtual InterfaceBase* queryInterface() = 0;
	//virtual void addRef() = 0;
	virtual void release() = 0;

protected:
	~Interface() { }
};

// Plugin - single static instance of each plugin is created when plugin library is loaded
class Plugin
{
public:
	virtual const char* name() const = 0;
	virtual unsigned int type() const = 0;

	virtual void link(Plugin* chain) = 0;
	virtual Plugin* next() const = 0;

//	static const unsigned int YValve = 1;
//	static const unsigned int Engine = 2;
//	static const unsigned int Redirector = 3;
	static const unsigned int AuthServer = 4;
	static const unsigned int AuthClient = 5;
	static const unsigned int UserManagement = 6;
};

} // namespace Firebird

extern "C"
{
	// additional API functions
	void ISC_EXPORT fb_register_plugin ( Firebird::Plugin* plugin );
	Firebird::Plugin* ISC_EXPORT fb_query_plugin (unsigned int type, const char* name);
}

#endif // FB_COMMON_CLASSES_INTERFACE
