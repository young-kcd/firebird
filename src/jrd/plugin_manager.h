/*
 *  plugin_manager.h
 *  firebird_test
 *
 *  Created by john on Wed Jan 09 2002.
 *  Copyright (c) 2001 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef PLUGIN_MANAGER_H
#define PLUGIN_MANAGER_H

#include "../jrd/os/mod_loader.h"
#include "fb_map.h"
#include "fb_list.h"
#include "fb_string.h"

class PluginManager
{
public:
	class iterator;
	
private:
	class Module
	{
	public:
		Module(const Firebird::string& name)
			: refCnt(0), module_name(name), prev(0), next(0) {}
		virtual ~Module();
		void aquire() { refCnt++; }
		/* 23 Sep 2002, Nickolay Samofatov commented it out because 
		  otherwise it caused nasty memory corruption. 
		  LoadAllPlugins function puts plugins with refCnt=0 to the list. 
		  I'd add destructor to the PluginManager to free memory...
		  */
		void release() { refCnt--; /*if (refCnt == 0) delete this;*/ }
		const Firebird::string& name() const { return module_name; }
		virtual void *lookupSymbol(Firebird::string&) = 0;
		
	private:
		friend class PluginManager;
		friend class iterator;
		int refCnt;
		Firebird::string module_name;
		Module **prev;
		Module *next;
		
		virtual void unload_module() {}
	};

public:
	friend class Plugin;
	friend class iterator;
	
	PluginManager() : moduleList(0) {}

	class Plugin
	{
	public:
		friend class PluginManager;
		Plugin() : module(0) {}
		Plugin(const Plugin& src) : module(src.module)
			{ if (module) module->aquire(); }
		~Plugin() { if (module) module->release(); }
		const Plugin& operator=(const Plugin&);
		bool operator ==(const Plugin &other) const
			{ return module == other.module; }
		bool operator !=(const Plugin &other) const
			{ return module != other.module; }
		void *lookupSymbol(Firebird::string &sym)
			{ return module ? module->lookupSymbol(sym) : 0; }
		operator bool() { return module != 0; }
		
	private:
		friend class iterator;
		Plugin(Module *mod) : module(mod)
			{ if (module) module->aquire(); }
		
		Module *module;
	};
	
	class iterator
	{
	public:
		friend class PluginManager;
		const iterator &operator++()
			{ if (curr) curr = curr->next; return *this; }
		Plugin operator*()
			{ return Plugin(curr); }
		bool operator==(const iterator& cmp) const
			{ return curr == cmp.curr; }
		bool operator!=(const iterator& cmp) const
			{ return curr != cmp.curr; }
		operator bool() const { return curr != 0; }
		const iterator &operator=(const iterator& other)
			{ curr = other.curr; return *this; }
		iterator(const iterator& other) : curr(other.curr) {}
			
	private:
		Module *curr;
		
		iterator(Module *start = 0) : curr(start) {}
	};
	
	Plugin findPlugin(const Firebird::string&);
	void addSearchPath(const Firebird::string&, bool = true);
	void removeSearchPath(const Firebird::string&, bool = true);
	void addIgnoreModule(const Firebird::string &mod)
		{ ignoreModules.push_back(mod); }
	void loadAllPlugins();
	iterator end() { return iterator(0); }
	iterator begin() { return iterator(moduleList); }
	
private:
	typedef std::pair<Firebird::string, bool> Path;
	Module *moduleList;
	Firebird::list<Path> searchPaths;
	Firebird::list<Firebird::string> ignoreModules;
	
	Module *loadPluginModule(const Firebird::string& name);
	Module *loadBuiltinModule(const Firebird::string& name);

	class BuiltinModule : public Module
	{
	public:
		BuiltinModule(const Firebird::string& name) : Module(name) {}
		
	private:
		Firebird::map<Firebird::string, void*> symbols;
		
		void *lookupSymbol(Firebird::string&);
	};
	
	class PluginModule : public Module
	{
	public:
		PluginModule(const Firebird::string &name, ModuleLoader::Module *mod)
			: Module(name), module(mod) {}
			
	private:
		ModuleLoader::Module *module;
		
		void unload_module();
		void *lookupSymbol(Firebird::string&);
	};
};

#endif
