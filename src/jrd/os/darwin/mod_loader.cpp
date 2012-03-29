/*
 *	PROGRAM:		JRD Module Loader
 *	MODULE:			mod_loader.cpp
 *	DESCRIPTION:	Darwin specific class for loadable modules.
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
 *  Paul Beach
 */

#include "firebird.h"

#include "../jrd/os/mod_loader.h"
#include "../../common.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dlfcn.h>
#include <mach-o/dyld.h>


void inline DebPrint(const char* s) 
{
	//printf("%s\n", s);
}

/// This is the Darwin implementation of the mod_loader abstraction.

class DarwinModule : public ModuleLoader::Module
{
public:
	DarwinModule(NSModule ns, void* dl) : nsModule(ns), dlModule(dl) { } 
	~DarwinModule();
	void *findSymbol(const Firebird::string&);
	
private:
	NSModule nsModule;
	void *dlModule;		// non-NULL means this is a dynamic library not a bundle
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
	Firebird::PathName::size_type pos = name.rfind(".dylib");
	if (pos != Firebird::PathName::npos && pos == name.length() - 6)
		return;		// No doctoring necessary
	name += ".dylib";
}

ModuleLoader::Module* ModuleLoader::loadModule(const Firebird::PathName& modPath)
{
	NSObjectFileImage image;

	/* Create an object file image from the given path */
	const NSObjectFileImageReturnCode retVal =
		NSCreateObjectFileImageFromFile(modPath.c_str(), &image);
	switch (retVal)
	{
	case NSObjectFileImageSuccess:
		break;
	case NSObjectFileImageFailure:
		DebPrint("object file setup failure");
		return 0;
	case NSObjectFileImageInappropriateFile:
		{
			// try to load as a dynamic library
			void* mod = dlopen(modPath.c_str(), RTLD_LAZY);
			if (mod)
			{
			return FB_NEW(*getDefaultMemoryPool()) DarwinModule(NSModule(), mod);
			}
			else
			{
			DebPrint("not a Mach-O MH_BUNDLE file type or dynamic library");
			return 0;	
			}
		}
	case NSObjectFileImageArch:
		DebPrint("no object for this architecture");
		return 0;
	case NSObjectFileImageFormat:
		DebPrint("bad object file format");
		return 0;
	case NSObjectFileImageAccess:
		DebPrint("can't read object file");
		return 0;
	default:
		DebPrint("unknown error from NSCreateObjectFileImageFromFile()");
		return 0;
	}
	
	/* link the image */
	NSModule mod_handle =
		NSLinkModule(image, modPath.c_str(), NSLINKMODULE_OPTION_PRIVATE);
	NSDestroyObjectFileImage(image);
	if (mod_handle == NULL)
	{
		DebPrint("NSLinkModule() failed for dlopen()");
		// We should really throw an error here.
		return 0;
	}
	
	NSSymbol initSym = NSLookupSymbolInModule(mod_handle, "__init");
	if (initSym != NULL)
	{
		void (*init)(void);
		init = ( void (*)(void)) NSAddressOfSymbol(initSym);
		init();
	}
	
	return FB_NEW(*getDefaultMemoryPool()) DarwinModule(mod_handle, 0);
}

DarwinModule::~DarwinModule()
{
	if (dlModule)
	{
		dlclose(dlModule);
	}
	else 
	{
		/* Make sure the fini function gets called, if there is one */
		NSSymbol symbol = NSLookupSymbolInModule(nsModule, "__fini");
		if (symbol != NULL)
		{
			void (*fini)(void);
			fini = (void (*)(void)) NSAddressOfSymbol(symbol);
			fini();
		}
		NSUnLinkModule (nsModule, 0);
	}
}

void *DarwinModule::findSymbol(const Firebird::string& symName)
{
	if (dlModule)
	{
		void *result = dlsym(dlModule, symName.c_str());
		if (result == NULL)
		{
			Firebird::string newSym = '_' + symName;
			result = dlsym(dlModule, newSym.c_str());
		}
		return result;
	}

	NSSymbol symbol = NSLookupSymbolInModule(nsModule, symName.c_str());
	if (symbol == NULL)
	{
		Firebird::string newSym = '_' + symName;
		symbol = NSLookupSymbolInModule(nsModule, newSym.c_str());
		if (symbol == NULL)
		{
			return NULL;
		}
	}
	return NSAddressOfSymbol(symbol);
}
