/*
 *	PROGRAM:		JRD Module Loader
 *	MODULE:			mod_loader.cpp
 *	DESCRIPTION:	POSIX-specific class for loadable modules.
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
 *  The Original Code was created by John Bellardo
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2002 John Bellardo <bellardo at cs.ucsd.edu>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 *
 */

#include "../jrd/os/mod_loader.h"
#include "../../common.h"
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <sys/types.h>
#include <sys/stat.h>
#include <dlfcn.h>

/// This is the POSIX (dlopen) implementation of the mod_loader abstraction.

class DlfcnModule : public ModuleLoader::Module
{
public:
	DlfcnModule(void *m) : module(m) {}
	~DlfcnModule();
	void *findSymbol(const Firebird::string&);
	
private:
	void *module;
};

bool ModuleLoader::isLoadableModule(const Firebird::PathName& module)
{
	struct stat sb;
	if (-1 == stat(module.c_str(), &sb))
		return false;
	if ( ! (sb.st_mode & S_IFREG) )		// Make sure it is a plain file
		return false;
	if ( -1 == access(module.c_str(), R_OK | X_OK))
		return false;
	return true;
}

void ModuleLoader::doctorModuleExtention(Firebird::PathName& name)
{
	Firebird::PathName::size_type pos = name.rfind(".so");
	if (pos != Firebird::PathName::npos && pos == name.length() - 3)
		return;		// No doctoring necessary
	name += ".so";
}

ModuleLoader::Module *ModuleLoader::loadModule(const Firebird::PathName& modPath)
{
	void* module = dlopen(modPath.c_str(), RTLD_LAZY);
	if (module == NULL)
		return 0;
	
	return FB_NEW(*getDefaultMemoryPool()) DlfcnModule(module);
}

DlfcnModule::~DlfcnModule()
{
	if (module)
		dlclose(module);
}

void *DlfcnModule::findSymbol(const Firebird::string& symName)
{
	void *result = dlsym(module, symName.c_str());
	if (result == NULL)
	{
		Firebird::string newSym = '_' + symName;

		result = dlsym(module, newSym.c_str());
	}
	return result;
}

