/*
 *	PROGRAM:		JRD Module Loader
 *	MODULE:			mod_loader.cpp
 *	DESCRIPTION:	Darwin-specific class for loadable modules.
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
#include "common.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <mach-o/dyld.h>

/// This is the Darwin implementation of the mod_loader abstraction.

class DarwinModule : public ModuleLoader::Module
{
public:
	DarwinModule(NSModule m) : module(m) {}
	~DarwinModule();
	void *findSymbol(const Firebird::string&);
	
private:
	NSModule module;
};

bool ModuleLoader::isLoadableModule(const Firebird::string& module)
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

void ModuleLoader::doctorModuleExtention(Firebird::string& name)
{
	Firebird::string::size_type pos = name.rfind(".dylib");
	if (pos != Firebird::string::npos && pos == name.length() - 6)
		return;		// No doctoring necessary
	name += ".dylib";
}

ModuleLoader::Module *ModuleLoader::loadModule(const Firebird::string& modPath)
{
	NSObjectFileImage image;

	/* Create an object file image from the given path */
	const NSObjectFileImageReturnCode retVal =
		NSCreateObjectFileImageFromFile(modPath.c_str(), &image);
	if (retVal != NSObjectFileImageSuccess)
	{
		switch (retVal)
		{
			case NSObjectFileImageFailure:
					/*printf("object file setup failure");*/
					return 0;
				case NSObjectFileImageInappropriateFile:
					/*printf("not a Mach-O MH_BUNDLE file type");*/
					return 0;
				case NSObjectFileImageArch:
					/*printf("no object for this architecture");*/
					return 0;
				case NSObjectFileImageFormat:
					/*printf("bad object file format");*/
					return 0;
				case NSObjectFileImageAccess:
					/*printf("can't read object file");*/
					return 0;
				default:
					/*printf("unknown error from NSCreateObjectFileImageFromFile()");*/
					return 0;
		}
	}
	
	/* link the image */
	NSModule mod_handle =
		NSLinkModule(image, modPath.c_str(), NSLINKMODULE_OPTION_PRIVATE);
	NSDestroyObjectFileImage(image);
	if (mod_handle == NULL)
	{
		/*printf("NSLinkModule() failed for dlopen()");*/
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
	
	return FB_NEW(*getDefaultMemoryPool()) DarwinModule(mod_handle);
}

DarwinModule::~DarwinModule()
{
	/* Make sure the fini function gets called, if there is one */
	NSSymbol symbol = NSLookupSymbolInModule(module, "__fini");
	if (symbol != NULL)
	{
		void (*fini)(void);
		fini = (void (*)(void)) NSAddressOfSymbol(symbol);
		fini();
	}   
	
	NSUnLinkModule (module, 0);
}

void *DarwinModule::findSymbol(const Firebird::string& symName)
{
	NSSymbol symbol = NSLookupSymbolInModule(module, symName.c_str());
	if (symbol == NULL)
	{
		Firebird::string newSym = '_' + symName;

		symbol = NSLookupSymbolInModule(module, newSym.c_str());
		if (symbol == NULL)
		{
			return NULL;
		}
	}
	return NSAddressOfSymbol(symbol);
}

