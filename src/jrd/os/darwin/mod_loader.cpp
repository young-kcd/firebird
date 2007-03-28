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
 *
 */
#include "../jrd/os/mod_loader.h"
#include "../../common.h"
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

ModuleLoader::Module *ModuleLoader::loadModule(const Firebird::PathName& modPath)
{
	NSObjectFileImage image;
	NSObjectFileImageReturnCode retVal;
	NSModule mod_handle;
	NSSymbol initSym;
	void (*init)(void);
	
	/* Create an object file image from the given path */
	retVal = NSCreateObjectFileImageFromFile(modPath.c_str(), &image);
	if (retVal != NSObjectFileImageSuccess)
	{
		switch(retVal)
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
	mod_handle = NSLinkModule(image, modPath.c_str(), NSLINKMODULE_OPTION_PRIVATE);
	NSDestroyObjectFileImage(image) ;
	if(mod_handle == NULL)
	{
		/*printf("NSLinkModule() failed for dlopen()");*/
		// We should really throw an error here.
		return 0;
	}
	
	initSym = NSLookupSymbolInModule(mod_handle, "__init");
	if (initSym != NULL)
	{
		init = ( void (*)(void)) NSAddressOfSymbol(initSym);
		init();
	}
	
	return FB_NEW(*getDefaultMemoryPool()) DarwinModule(mod_handle);
}

DarwinModule::~DarwinModule()
{
	NSSymbol symbol;  
	void (*fini)(void);
	
	/* Make sure the fini function gets called, if there is one */
	symbol = NSLookupSymbolInModule(module, "__fini");
	if (symbol != NULL)
	{
		fini = (void (*)(void)) NSAddressOfSymbol(symbol);
		fini();
	}   
	
	NSUnLinkModule (module, 0);
}

void *DarwinModule::findSymbol(const Firebird::string& symName)
{
	NSSymbol symbol;

	symbol = NSLookupSymbolInModule(module, symName.c_str());
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
