/*
 *  PROGRAM:    	JRD Plugin Manager Class
 *  MODULE:     	plugin_manager.h
 *  DESCRIPTION:    Manages dynamically loadable engine plugins
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
 *  Copyright (c) 2002 John Bellardo <bellardo@cs.ucsd.edu>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 */

#ifndef JRD_PLUGIN_MANAGER_H
#define JRD_PLUGIN_MANAGER_H

#include "../jrd/os/mod_loader.h"
#include "../common/classes/fb_string.h"
#include "../common/classes/fb_pair.h"
#include "../common/classes/objects_array.h"
#include "../common/classes/init.h"

/** The PluginManager class provides a platform independent interface for
  *   loading, unloading, and accessing symbols in dynamically loadable
  *   libraries.  The PluginManager has the ability to restrict module
  *   loading to specific search paths, ignore certain modules in a
  *   search path, and automatically load all the modules in the search
  *   paths.
  *
  * In addition, the PluginManager provides for a single, unified way of
  *   handling loadable modules.  This is to prevent an explosion of
  *   separate PluginManager objects as the engine grows.  The pre-existing
  *   subsystems that used loadable modules (for example: intl, flu) will still
  *   have their own PluginManager instance.  But other subsystems should try
  *   to stay within the boundries of the new, more generic, plugin interface.
  *
  * This interface is divided into two parts: enginge and plugin.  The engine
  *   interface consists of a single function: load_engine_plugins.  This
  *   function searches through the plugin directory (XXX) and loads all the
  *   plugins it finds.  After loading it calls the plugin's "register" function.
  *   This function has the signature "extern "C" void register_plugin(void *parm)".
  *   This function is, in turn, responsible for registering the plugin with the
  *   various parts of the engine as required.  To do this each engine subsystem
  *   that wants to make use of plugins needs to provide a "registration" function.
  *   This function can take any parameters as necessary to establish the link between
  *   the plugin and the subsystem, but it must accept the void* parameter given to
  *   the plugin's register function.  This parameter is a Plugin pointer which allows
  *   the subsystem to access the plugin's symbols.
  *
  * The exact semantics of the function the plugin needs to call varies by 
  *    subsystem.  Please refer to the subsystem documentation for details.
  **/

class PluginManager : public Firebird::PermanentStorage
{
	static const char *ENGINE_PLUGIN_DIR;
	static const char *ENGINE_PLUGIN_REGISTRATION_ENTRYPOINT;

public:
	static void load_engine_plugins();
	static Firebird::InitInstance<PluginManager> enginePluginManager;

	class iterator;
	
private:
	/** PluginManager::Module is an internal class that encapsulates
	  *  the reference counter, module array, module
	  *  name, and symbol lookup function of a loaded module.  This class
	  *  is expected to be subclassed to provide module specific functionality.
	  *  For example, a builtin module class may load modules that have been statically
	  *  linked in with the engine, while a dynamic module class might handle modules
	  *  found in OS specific dynamic libraries.
	  **/
	class Module
	{
	public:
		/// The constructor requires the name of the module.  The initial reference count
		///  is 1, to indicate that the PluginManager itself has a reference.  This is
		///  verified in debug builds in the PluginManager destructor.
		Module(MemoryPool& p, const Firebird::PathName& nameL)
			: refCnt(1), module_name(p, nameL), prev(0), next(0) {}
		/// The destructor is responsible for removing the module from the array.
		virtual ~Module();
		/// aquire indicates the interest of a particular piece of engine code in
		///  the loadable module.  Currently that interest is tracked via a reference
		///  counter.
		void aquire() { refCnt++; }
		/// release indicates the end of interest in a modules.  Each call to aquire
		///  must be balanced with a call to release.
		void release() { refCnt--; }
		/// Accessor function that returns the name of the module
		const Firebird::PathName& name() const { return module_name; }
		/// lookupSymbol searches through a module's symbol table and attempts to
		///  locate the given symbol name.  If successful it returns a pointer to
		///  the symbol's location in the current address space.  If the symbol can't
		///  be found it returns 0.
		virtual void *lookupSymbol(const Firebird::string&) = 0;
		
	private:
		friend class PluginManager;
		friend class iterator;
		int refCnt;
		Firebird::PathName module_name;
		Module **prev;
		Module *next;
		
		/// unload_module is used called by the destructor when the module
		/// represented by this module object needs to be physically unloaded
		/// from memory.
		virtual void unload_module() {}
	};

public:
	friend class Plugin;
	friend class iterator;
	
	explicit PluginManager(MemoryPool& p) : Firebird::PermanentStorage(p), 
		moduleList(0), searchPaths(getPool()), ignoreModules(getPool()) {}
	~PluginManager();

	/** The Plugin class encapsulates the various operations that are available
	  *  after a plugin has been located.  The PluginManager returns instances
	  *  of the Plugin class to the calling code to give that code access to
	  *  the aforementioned operations.  These operations include tracking the
	  *  usage of a plugin, and locating symbols within the plugin.  The Plugin
	  *  class has a full contingent of constructors, assignment operators, and
	  *  equality operators.
	  **/
	class Plugin
	{
	public:
		friend class PluginManager;
		Plugin() : module(0) {}
		Plugin(const Plugin& src) : module(src.module)
		{
			if (module)
				module->aquire();
		}
		~Plugin()
		{
			if (module)
				module->release();
		}
		const Plugin& operator=(const Plugin&);
		bool operator ==(const Plugin &other) const
			{ return module == other.module; }
		bool operator !=(const Plugin &other) const
			{ return module != other.module; }
		/// Looks in the Plugin's symbol table for a symbol with the given name.
		///  If the symbol exists a pointer to that symbol's location in the
		/// current address space is returned.  Otherwise NULL is returned.
		void *lookupSymbol(const Firebird::string &sym)
			{ return module ? module->lookupSymbol(sym) : 0; }
		operator bool() { return module != 0; }
		
	private:
		friend class iterator;
		Plugin(Module *mod) : module(mod)
		{
			if (module)
				module->aquire();
		}
		
		Module *module;
	};
	
	/** The iterator class allows external code to iterate through all the plugins
	  *  a plugin manager knows about.  The iterator behaves like a traditional STL
	  *  iterator.
	  **/
	class iterator
	{
	public:
		friend class PluginManager;
		const iterator &operator++()
		{
			if (curr)
				curr = curr->next;
			return *this;
		}
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
	
	/// Searches for the plugin with the given name using the
	///  preset search criteria.
	Plugin findPlugin(const Firebird::PathName&);
	/// Adds a path to the array of paths to be searched for a modules.  The
	///  second parameter indicates if the path is absolute, or relative to
	///  the current working directory.
	void addSearchPath(const Firebird::PathName&, bool = true);
	/// Removes the given search path from the array of search paths.  The path name
	/// (1st parameter) and the relative indicator (2nd parameter) must exactly match
	/// those passed in to addSearchPath.
	void removeSearchPath(const Firebird::PathName&, bool = true);
	/// Adds a module to the ignore list.  Ignored modules are not loaded automatically
	///  when a directory is scanned, but may be loaded manually via the findPlugin
	///  function.
	void addIgnoreModule(const Firebird::PathName &mod)
		{ ignoreModules.push(mod); }
	/// Loads all the plugins found in the current set of search paths, except those that
	///  have been ignored.  This function must be called after adding all the needed search paths,
	///  and before iterating through the plugins.
	void loadAllPlugins();
	/// The traditional STL iterator end function.  Returns an iterator that represents the
	/// last element in the iteration.
	iterator end() { return iterator(0); }
	/// Returns an iterator set to the first element in the iteration set.  That set
	///  is the set of all plugins known to the PluginManager.  This includes all plugins
	///  found with the loadAllPluings and findPlugin functions.
	iterator begin() { return iterator(moduleList); }

private:
	typedef void (*engineRegistrationFuncType)(Plugin*);
	typedef Firebird::Pair<Firebird::Left<Firebird::PathName, bool> > Path;
	Module *moduleList;
	Firebird::ObjectsArray<Path> searchPaths;
	typedef Firebird::ObjectsArray<Path>::iterator spIterator;
	Firebird::ObjectsArray<Firebird::PathName> ignoreModules;
	
	Module *loadPluginModule(const Firebird::PathName& name);
	Module *loadBuiltinModule(const Firebird::PathName& name);

	class BuiltinModule : public Module
	{
	public:
		BuiltinModule(MemoryPool& p, const Firebird::PathName& nameL) 
			: Module(p, nameL), symbols(p) {}
		
	private:
		typedef Firebird::Pair<Firebird::Left<Firebird::string, void*> > Symbol;
		typedef Firebird::FirstKey<Symbol> SymbolKey;
		typedef Firebird::SortedArray<Symbol, 
			Firebird::EmptyStorage<Symbol>, 
			Firebird::string, SymbolKey> Symbols;
		Symbols symbols;
		
		void *lookupSymbol(const Firebird::string&);
	};
	
	class PluginModule : public Module
	{
	public:
		PluginModule(MemoryPool& p, const Firebird::PathName &nameL, ModuleLoader::Module *mod)
			: Module(p, nameL), module(mod) {}
			
	private:
		ModuleLoader::Module *module;
		
		void unload_module();
		void *lookupSymbol(const Firebird::string&);
	};
};

#endif // JRD_PLUGIN_MANAGER_H

