/*
 *  mod_loader.cpp
 *
 */

#include "../jrd/os/mod_loader.h"
#include <windows.h>

/// This is the Win32 implementation of the mod_loader abstraction.

class Win32Module : public ModuleLoader::Module
{
public:
	Win32Module(HMODULE m) : module(m) {}
	~Win32Module();
	void *findSymbol(const Firebird::string&);
	
private:
	HMODULE module;
};

bool ModuleLoader::isLoadableModule(const Firebird::string& module)
{
	return (GetModuleHandle(module.c_str()) || LoadLibrary(module.c_str()));
}

void ModuleLoader::doctorModuleExtention(Firebird::string& name)
{
	Firebird::string::size_type pos = name.rfind(".dll");
	if (pos != Firebird::string::npos && pos == name.length() - 4)
		return;		// No doctoring necessary
	name += ".dll";
}

ModuleLoader::Module *ModuleLoader::loadModule(const Firebird::string& modPath)
{
	HMODULE module = GetModuleHandle(modPath.c_str());
	if (!module)
		module = LoadLibrary(modPath.c_str());
	if (!module)
		return 0;
	
	return FB_NEW(*getDefaultMemoryPool()) Win32Module(module);
}

Win32Module::~Win32Module()
{
	if (module)
		FreeLibrary(module);
}

void *Win32Module::findSymbol(const Firebird::string& symName)
{
	void *result;  

	result = GetProcAddress(module, symName.c_str());
	if (!result)
	{
		Firebird::string newSym = '_' + symName;

		result = GetProcAddress(module, newSym.c_str());
	}
	return result;
}
