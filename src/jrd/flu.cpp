/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		flu.cpp
 *	DESCRIPTION:	Function Lookup Code
 *
 * The contents of this file are subject to the Interbase Public
 * License Version 1.0 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy
 * of the License at http://www.Inprise.com/IPL.html
 *
 * Software distributed under the License is distributed on an
 * "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express
 * or implied. See the License for the specific language governing
 * rights and limitations under the License.
 *
 * The Original Code was created by Inprise Corporation
 * and its predecessors. Portions created by Inprise Corporation are
 * Copyright (C) Inprise Corporation.
 *
 * All Rights Reserved.
 * Contributor(s): ______________________________________.
 *
 * 2002.02.15 Sean Leyne - Code Cleanup, removed obsolete "EPSON" define
 *
 * 2002-02-23 Sean Leyne - Code Cleanup, removed old M88K and NCR3000 port
 *
 * 2002.10.27 Sean Leyne - Code Cleanup, removed obsolete "UNIXWARE" port
 *
 * 2002.10.28 Sean Leyne - Completed removal of obsolete "DGUX" port
 * 2002.10.28 Sean Leyne - Code cleanup, removed obsolete "DecOSF" port
 * 2002.10.28 Sean Leyne - Code cleanup, removed obsolete "SGI" port
 * 2002.10.28 Sean Leyne - Completed removal of obsolete "HP700" port
 *
 * 2002.11.28 Sean Leyne - Code cleanup, removed obsolete "HM300" port
 *
 * 2003.04.12 Alex Peshkoff - Security code cleanup:
 *		1.	Drop EXT_LIB_PATH verification
 *		2.	Drop IB_UDF_DIR & IB_INTL_DIR support
 *		3.	Created common for all platforms search_for_module,
 *			using dir_list and PathUtils. Platform specific
 *			macros defined after ISC-lookup-entrypoint()
 *			for each platform.
 *
 * 2004.11.27 Alex Peshkoff - removed results of Borland's copyNpaste
 *		programming style. Almost all completely rewritten.
 *
 */
/*
$Id: flu.cpp,v 1.48 2004-11-29 11:15:08 alexpeshkoff Exp $
*/

#include "firebird.h"
#include "../common/config/config.h"
#include "../common/config/dir_list.h"
#include "../jrd/os/path_utils.h"
#include "../common/classes/init.h"
#include "../jrd/jrd.h"

#include "../jrd/common.h"
#include "../jrd/flu.h"
#include "../jrd/gdsassert.h"

#include "../jrd/flu_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/dls_proto.h"
#include "../jrd/err_proto.h"

#include "gen/iberror.h"

#include <string.h>

#if (defined SOLARIS || defined SCO_EV || defined LINUX || defined AIX_PPC || defined SINIXZ || defined FREEBSD || defined NETBSD)
#define DYNAMIC_SHARED_LIBRARIES
#endif


namespace {
	Firebird::InitInstance<Jrd::Module::LoadedModules> loadedModules;
	Firebird::Mutex modulesMutex;

	template <typename S>
	void terminate_at_space(S& s, const char* psz)
	{
		const char *p = psz;
		while (*p && *p != ' ')
		{
			++p;
		}
		s.assign(psz, p - psz);
	}

	// we have to keep prefix/suffix per-OS mechanism
	// here to help dir_list correctly locate module
	// in one of it's directories

	enum ModKind {MOD_PREFIX, MOD_SUFFIX};
	struct Libfix {
		ModKind kind;
		const char* txt;
		bool permanent;
	};

	const Libfix libfixes[] = {

#ifdef WIN_NT
// to avoid implicit .dll suffix
		{MOD_SUFFIX, ".", false},
#endif

// always try to use module "as is"
		{MOD_SUFFIX, "", false},

#ifdef HP10
		{MOD_SUFFIX, ".sl", true},
#endif

#ifdef DYNAMIC_SHARED_LIBRARIES
		{MOD_SUFFIX, ".so", true},
		{MOD_PREFIX, "lib", true},
#endif

#ifdef WIN_NT
		{MOD_SUFFIX, ".DLL", true},
#endif

#ifdef DARWIN
		{MOD_SUFFIX, ".so", true},
#endif

	};

	// UDF/BLOB filter verifier
	class UdfDirectoryList : public Firebird::DirectoryList
	{
	private:
		const Firebird::PathName getConfigString(void) const {
			return Firebird::PathName(Config::getUdfAccess());
		}
	public:
		UdfDirectoryList(MemoryPool& p) : DirectoryList(p) 
		{
			initialize();
		}
	};
	Firebird::InitInstance<UdfDirectoryList> iUdfDirectoryList;
}

namespace Jrd
{
	Module::InternalModule* Module::scanModule(const Firebird::PathName& name)
	{
		typedef Module::InternalModule** itr;
		for (itr it = loadedModules().begin(); it != loadedModules().end(); ++it)
		{
			if (**it == name)
			{
				return *it;
			}
		}
		return 0;
	}


	FPTR_INT Module::lookup(const TEXT* module, 
							const TEXT* name, 
							DatabaseModules& interest)
	{
		FPTR_INT function = FUNCTIONS_entrypoint(module, name);
		if (function)
		{
			return function;
		}

		// Try to find loadable module
		Module m = lookupModule(module, true);
		if (! m)
		{
			return 0;
		}

		Firebird::string symbol;
		terminate_at_space(symbol, name);
		void* rc = m.lookupSymbol(symbol);
		if (rc)
		{
			size_t pos;
			if (!interest.find(m, pos))
			{
				interest.add(m);
			}
		}

		return (FPTR_INT)rc;
	}

	FPTR_INT Module::lookup(const TEXT* module, 
							const TEXT* name)
	{
		FPTR_INT function = FUNCTIONS_entrypoint(module, name);
		if (function)
		{
			return function;
		}

		// Try to find loadable module
		Module m = lookupModule(module, false);
		if (! m)
		{
			return 0;
		}

		Firebird::string symbol;
		terminate_at_space(symbol, name);
		return (FPTR_INT)(m.lookupSymbol(symbol));
	}

	// flag 'udf' means pass name-path through UdfDirectoryList
	Module Module::lookupModule(const char* name, bool udf)
	{
		Firebird::MutexLockGuard lg(modulesMutex);
		Firebird::PathName initialModule;
		terminate_at_space(initialModule, name);

		// Look for module in array of already loaded 
		InternalModule* im = scanModule(initialModule);
		if (im)
		{
			return Module(im);
		}

		// apply suffix (and/or prefix) and try that name
		Firebird::PathName module(initialModule);
		for (int i=0; i<sizeof(libfixes)/sizeof(Libfix); i++)
		{
			const Libfix* l = &libfixes[i];
			// os-dependent module name modification
			Firebird::PathName fixedModule(module);
			switch (l->kind)
			{
			case MOD_PREFIX:
				fixedModule = l->txt + fixedModule;
				break;
			case MOD_SUFFIX:
				fixedModule += l->txt;
			}
			if (l->permanent)
			{
				module = fixedModule;
			}

			// Look for module with fixed name
			im = scanModule(fixedModule);
			if (im)
			{
				return Module(im);
			}

			if (udf)
			{
				// UdfAccess  verification
				Firebird::PathName path, relative;

				// Search for module name in UdfAccess restricted 
				// paths list
				PathUtils::splitLastComponent(path, relative, fixedModule);
				if (path.length() == 0 && 
						PathUtils::isRelative(fixedModule)) {
					path = fixedModule;
					iUdfDirectoryList().expandFileName(fixedModule, path);
				}

				// The module name, including directory path,
				// must satisfy UdfAccess entry in config file.
				bool ok = iUdfDirectoryList().isPathInList(fixedModule);

				ModuleLoader::Module* mlm = ModuleLoader::loadModule(fixedModule);
				if (mlm)
				{
					if (! ok)
					{
						delete mlm;
						// module loadable, but prohibited in configuration
						ERR_post(isc_conf_access_denied,
							isc_arg_string, "UDF/BLOB-filter module",
							isc_arg_string, ERR_cstring(fixedModule.c_str()),
							isc_arg_end);
					}
					im = FB_NEW(*getDefaultMemoryPool())
						InternalModule(*getDefaultMemoryPool(), mlm,
							initialModule, fixedModule);
					loadedModules().add(im);
					return Module(im);
				}
			}
			else
			{
				// try to load permanent module
				ModuleLoader::Module* mlm = ModuleLoader::loadModule(fixedModule);
				if (mlm)
				{
					im = FB_NEW(*getDefaultMemoryPool())
						InternalModule(*getDefaultMemoryPool(), mlm,
							initialModule, fixedModule);
					loadedModules().add(im);
					im->aquire();	// make permanent
					return Module(im);
				}
			}
		}
		// let others raise 'missing library' error if needed
		return Module();
	}

	Module::~Module()
	{
		if (interMod)
		{
			interMod->release();
			if (! interMod->inUse())
			{
				Firebird::MutexLockGuard lg(modulesMutex);
				for (size_t m = 0; m < loadedModules().getCount(); m++)
				{
					if (loadedModules()[m] == interMod)
					{
						loadedModules().remove(m);
						delete interMod;
						return;
					}
				}
				fb_assert(false);
				// In production server we delete interMod here
				// (though this is not normal case)
				delete interMod;
			}
		}
	}

} // namespace Jrd



// ********************************************************** //

// VMS stuff is kept in order someone would like to implement
// VMS-style mod_loader. AP.

/* VMS Specific Stuff */

#ifdef VMS

#include <descrip.h>
#include <ssdef.h>

static int condition_handler(int *, int *, int *);

FPTR_INT ISC-lookup-entrypoint(TEXT* module,
							   TEXT* name,
							   const TEXT* ib_path_env_var,
							   bool ShowAccessError)
{
/**************************************
 *
 *	I S C _ l o o k u p _ e n t r y p o i n t  ( V M S )
 *
 **************************************
 *
 * Functional description
 *	Lookup entrypoint of function.
 *
 **************************************/
	struct dsc$descriptor mod_desc, nam_desc;
	TEXT absolute_module[MAXPATHLEN];

	FPTR_INT function = FUNCTIONS_entrypoint(module, name);
	if (function)
		return function;

	if (ib_path_env_var == NULL)
		strcpy(absolute_module, module);
	else
		if (!gds__validate_lib_path
			(module, ib_path_env_var, absolute_module, sizeof(absolute_module)))
		{
			return NULL;
		}

	REPLACE THIS COMPILER ERROR WITH CODE TO VERIFY THAT THE MODULE IS FOUND
		EITHER IN $INTERBASE:UDF, or $INTERBASE:intl,
		OR IN ONE OF THE DIRECTORIES NAMED IN EXTERNAL_FUNCTION_DIRECTORY
		LINES IN ISC_CONFIG.for (p = absolute_module; *p && *p != ' '; p++);

	ISC_make_desc(absolute_module, &mod_desc, p - absolute_module);

	const TEXT* p = name;
	while (*p && *p != ' ')
	{
		++p;
	}

	ISC_make_desc(name, &nam_desc, p - name);
	VAXC$ESTABLISH(condition_handler);

	if (!(lib$find_image_symbol(&mod_desc, &nam_desc, &function, NULL) & 1))
		return NULL;

	return function;
}

static int condition_handler(int *sig, int *mech, int *enbl)
{
/**************************************
 *
 *	c o n d i t i o n _ h a n d l e r
 *
 **************************************
 *
 * Functional description
 *	Ignore signal from "lib$find_symbol".
 *
 **************************************/

	return SS$_CONTINUE;
}
#endif
