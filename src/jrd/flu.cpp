/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		flu.c
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
 */
/*
$Id: flu.cpp,v 1.30 2003-04-03 13:52:38 brodsom Exp $
*/

#include "firebird.h"

#include <string>

using namespace std;

#include "../jrd/common.h"
#include "../jrd/flu.h"
#include "../jrd/gdsassert.h"

#ifdef SHLIB_DEFS
// TMN: Why the ... declare this one in the middle of a buch of includes?!
extern "C" {
#define FUNCTIONS_entrypoint	(*_libgds_FUNCTIONS_entrypoint)
}
#endif

#include "../jrd/flu_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/dls_proto.h"
#include <string.h>


/* VMS Specific Stuff */

#ifdef VMS

#include <descrip.h>
#include <ssdef.h>

static int condition_handler(int *, int *, int *);

#endif


/* HP-UX specific stuff */

#ifdef HP10
#include <dl.h>
#include <shl.h>
#include <unistd.h>
#include <libgen.h>
#define IB_UDF_DIR 	"UDF/"
#endif

#if (defined SOLARIS || defined SCO_EV || defined LINUX || defined AIX_PPC || defined SINIXZ || defined FREEBSD)
#include <dlfcn.h>
#define DYNAMIC_SHARED_LIBRARIES
#include <unistd.h>
#include <libgen.h>
#define IB_UDF_DIR              "UDF/"
#endif

#ifdef DARWIN
#include <unistd.h>
#include <mach-o/dyld.h>
#include <libgen.h>
#define IB_UDF_DIR              "UDF/"
#endif

#if defined NETBSD
#include <dlfcn.h>
#define DYNAMIC_SHARED_LIBRARIES
#include <unistd.h>
#define IB_UDF_DIR              "UDF/"
#endif

#ifndef HAVE_DIRNAME
/*
 * Define our own dirname(), because we don't have a syscall for it.
 * !! WARNING !! WARNING !! WARNING !!
 * I'm hoping that the use of static result is OK here, and justify
 * its use by noting that A) we're not using threads, and B) callers
 * don't seem to hang onto the result for very long.  Since none of
 * its callers ever have to clean up anyway, I'm assuming that real
 * syscall versions do it the same way, except that they probably offer
 * thread-safeness as well.
 */
const char*
dirname(const char* fname)
{
	static char result[MAXPATHLEN];
	int i, last;

	/* Set default return value */
	result[0] = '.';
	result[1] = '\0';

	if (fname != (const char *)0 && *fname != '\0')
	{
		i = last = 0;
		while (*fname)
		{
			i++;
			if (*fname == '/')
			{
				last = i;
			}
			fname++;
		}
		if (last != 0)
		{
			/* Found dir seperator */
			assert(last < (int) sizeof(result));
			strncpy(result, fname, last);
			result[last] = '\0';
		}
	}

	return result;
}
#endif /* HAVE_DIRNAME */


static void terminate_at_space(char* psz)
{
	while (*psz && *psz != ' ')
	{
		++psz;
	}
	if (*psz) {
		*psz = '\0';
	}
}


/* Windows NT stuff */

#ifdef WIN_NT
#define NOUSER
#define NOGDI
#define NOCRYPT
#define NOMCX
#define NOIME
#ifndef NOMSG
#define NOMSG
#endif
#define NOSERVICE
#include <windows.h>
#include <stdlib.h>
#include <io.h>
#define IB_UDF_DIR		"UDF\\"
/* Where is the international character set module found? */
#define IB_INTL_DIR		"intl\\"
#endif


extern "C" {


/* Where is the international character set module found? */
#ifndef IB_INTL_DIR
#define IB_INTL_DIR		"intl/"
#endif

static MOD FLU_modules = 0;		/* External function/filter modules */

/* prototypes for private functions */
static MOD search_for_module(TEXT *, TEXT *);
#ifdef WIN_NT
static void adjust_loadlib_name(TEXT *, TEXT *);
#endif



MOD FLU_lookup_module(TEXT* module)
{
/**************************************
 *
 *	F L U _ l o o k u p _ m o d u l e
 *
 **************************************
 *
 * Functional description
 *	Lookup external function module descriptor.
 *
 **************************************/

#ifndef WIN_NT
	terminate_at_space(module);
#endif

	const USHORT length = strlen(module);

	for (MOD mod = FLU_modules; mod; mod = mod->mod_next)
	{
		if (mod->mod_length == length && !strcmp(mod->mod_name, module))
		{
			return mod;
		}
	}

	return 0;
}


void FLU_unregister_module(MOD module)
{
/**************************************
 *
 *	F L U _ u n r e g i s t e r _ m o d u l e
 *
 **************************************
 *
 * Functional description
 *	Unregister interest in an external function module.
 *	Unload module if it's uninteresting.
 *
 **************************************/
	MOD *mod;
	#ifdef DARWIN
	NSSymbol symbol;
	void (*fini)(void);
	#endif


/* Module is in-use by other databases.*/

	if (--module->mod_use_count > 0)
		return;

/* Unlink from list of active modules, unload it, and release memory. */

	for (mod = &FLU_modules; *mod; mod = &(*mod)->mod_next)
	{
		if (*mod == module)
		{
			*mod = module->mod_next;
			break;
		}
	}

#ifdef HP10
	shl_unload(module->mod_handle);
#endif

#if defined(SOLARIS) || defined(LINUX) || defined(FREEBSD) || defined(NETBSD) || defined (AIX_PPC) || defined(SINIXZ)
	dlclose(module->mod_handle);
#endif
#ifdef WIN_NT
	FreeLibrary(module->mod_handle);
#endif

#ifdef DARWIN
	/* Make sure the fini function gets called, if there is one */
	symbol = NSLookupSymbolInModule(module->mod_handle, "__fini");
	if (symbol != NULL)
	{
		fini = (void (*)(void)) NSAddressOfSymbol(symbol);
		fini();
	}
	NSUnLinkModule (module->mod_handle, 0);
#endif

	gds__free(module);
}


#ifdef VMS
#define LOOKUP
FPTR_INT ISC_lookup_entrypoint(TEXT * module,
							   TEXT * name, TEXT * ib_path_env_var)
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
	FPTR_INT function;
	TEXT *p;
	struct dsc$descriptor mod_desc, nam_desc;
	TEXT absolute_module[MAXPATHLEN];

	if (function = FUNCTIONS_entrypoint(module, name))
		return function;

	if (ib_path_env_var == NULL)
		strcpy(absolute_module, module);
	else
		if (!gds__validate_lib_path
			(module, ib_path_env_var, absolute_module,
			 sizeof(absolute_module))) return NULL;

	REPLACE THIS COMPILER ERROR WITH CODE TO VERIFY THAT THE MODULE IS FOUND
		EITHER IN $INTERBASE:UDF, or $INTERBASE:intl,
		OR IN ONE OF THE DIRECTORIES NAMED IN EXTERNAL_FUNCTION_DIRECTORY
		LINES IN ISC_CONFIG.for (p = absolute_module; *p && *p != ' '; p++);

	ISC_make_desc(absolute_module, &mod_desc, p - absolute_module);

	p = name;
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
#endif


/*
  The commented #if which immediately follows has been replaced.
  The OLD_AIX and OLD_AIX_PPC #if has been added to disable the
  code segment for current AIX builds.
*/
/*
#if (defined AIX || defined AIX_PPC)
*/
#if (defined OLD_AIX || defined OLD_AIX_PPC)
#define LOOKUP
FPTR_INT ISC_lookup_entrypoint(TEXT* module,
							   TEXT* name,
							   TEXT* ib_path_env_var)
{
/**************************************
 *
 *	I S C _ l o o k u p _ e n t r y p o i n t  ( A I X )
 *
 **************************************
 *
 * Functional description
 *	Lookup entrypoint of function.
 *
 **************************************/
	FPTR_INT function;
	TEXT *p;
	TEXT absolute_module[MAXPATHLEN];

	function = FUNCTIONS_entrypoint(module, name);
	if (function)
	{
		return function;
	}

	terminate_at_space(module);

	if (ib_path_env_var == NULL)
	{
		strcpy(absolute_module, module);
	}
	else
	{
		if (!gds__validate_lib_path(module,
									ib_path_env_var,
									absolute_module,
									sizeof(absolute_module)))
		{
			return NULL;
		}
	}

	REPLACE THIS COMPILER ERROR WITH CODE TO VERIFY THAT THE MODULE IS FOUND
		EITHER IN $INTERBASE / UDF, OR $INTERBASE / intl,
		OR IN ONE OF THE DIRECTORIES NAMED IN EXTERNAL_FUNCTION_DIRECTORY
		LINES IN ISC_CONFIG.function = load(absolute_module, 0, NULL);

	return function;
}
#endif


#ifdef HP10
#define LOOKUP
FPTR_INT ISC_lookup_entrypoint(TEXT * module,
							   TEXT * name, TEXT * ib_path_env_var)
{
/**************************************
 *
 *	I S C _ l o o k u p _ e n t r y p o i n t  ( H P 9 0 0 0 s 7 0 0 )
 *
 **************************************
 *
 * Functional description
 *	Lookup entrypoint of function.
 *
 **************************************/
	FPTR_INT function;
	TEXT *p;

	function = FUNCTIONS_entrypoint(module, name);
	if (function)
	{
		return function;
	}

	terminate_at_space(module);
	terminate_at_space(name);

	if (!*module || !*name)
	{
		return NULL;
	}

	// Check if external function module has already been loaded
	MOD mod = FLU_lookup_module(module);

	if (!mod)
	{
		TEXT absolute_module[MAXPATHLEN];

#ifdef EXT_LIB_PATH
		if (ib_path_env_var == NULL)
		{
			strcpy(absolute_module, module);
		}
		else
		{
			if (!gds__validate_lib_path(module,
										ib_path_env_var,
										absolute_module,
										sizeof(absolute_module)))
			{
				return NULL;
			}
		}
#else
		strcpy(absolute_module, module);
#endif /* EXT_LIB_PATH */

		const USHORT length = strlen(absolute_module);

		/* call search_for_module with the supplied name,
		   and if unsuccessful, then with <name>.sl . */
		mod = search_for_module(absolute_module, name);
		if (!mod)
		{
			strcat(absolute_module, ".sl");
			mod = search_for_module(absolute_module, name);
		}
		if (!mod)
		{
			return NULL;
		}

		assert(mod->mod_handle);	/* assert that we found the module */
		mod->mod_use_count = 0;
		mod->mod_length = length;
		strcpy(mod->mod_name, module);
		mod->mod_next = FLU_modules;
		FLU_modules = mod;
	}

	++mod->mod_use_count;

	if (shl_findsym(&mod->mod_handle, name, TYPE_PROCEDURE, &function))
		return NULL;

	return function;
}


static MOD search_for_module(TEXT * module, TEXT * name)
{
/**************************************
 *
 *	s e a r c h _ f o r _ m o d u l e	( H P 9 0 0 0 s 7 0 0 )
 *
 **************************************
 *
 * Functional description
 *	Look for a module (as named in a 'DECLARE EXTERNAL FUNCTION'
 *      statement.
 *
 **************************************/
	MOD mod;
	char *dirp;
	TEXT ib_lib_path[MAXPATHLEN];
	TEXT absolute_module[MAXPATHLEN];
	FDLS *dir_list;
	BOOLEAN found_module;

	strcpy(absolute_module, module);

	if (!(mod = (MOD) gds__alloc(sizeof(struct mod) +
								 strlen(absolute_module)))) return NULL;

	dirp = dirname(absolute_module);
	if (('.' == dirp[0]) && ('\0' == dirp[1]))
	{
		/*  We have a simple module name, without a directory. */

		gds__prefix(ib_lib_path, IB_UDF_DIR);
		strncat(ib_lib_path, module, MAXPATHLEN - strlen(ib_lib_path) - 1);
		if (!access(ib_lib_path, R_OK | X_OK))
		{
			/* Module is in the default UDF directory: load it. */
			if (!(mod->mod_handle = shl_load(ib_lib_path, BIND_DEFERRED, 0)))
			{
				gds__free(mod);
				return NULL;
			}
		}
		else
		{
			gds__prefix(ib_lib_path, IB_INTL_DIR);
			strncat(ib_lib_path, module,
					MAXPATHLEN - strlen(ib_lib_path) - 1);
			if (!access(ib_lib_path, R_OK | X_OK))
			{
				/* Module is in the international character set directory:
				   load it. */
				mod->mod_handle = shl_load(ib_lib_path, BIND_DEFERRED, 0);
				if (!mod->mod_handle)
				{
					gds__free(mod);
					return NULL;
				}
			}
			else
			{
				/*  The module is not in a default directory, so ...
				 *  use the EXTERNAL_FUNCTION_DIRECTORY lines from isc_config.
				 */
				dir_list = DLS_get_func_dirs();
				found_module = FALSE;
				while (dir_list && !found_module)
				{
					strcpy(ib_lib_path, dir_list->fdls_directory);
					strcat(ib_lib_path, "/");
					strncat(ib_lib_path, module,
							MAXPATHLEN - strlen(ib_lib_path) - 1);
					if (!access(ib_lib_path, R_OK | X_OK))
					{
						mod->mod_handle = shl_load(ib_lib_path,
													 BIND_DEFERRED, 0);
						if (!mod->mod_handle)
						{
							gds__free(mod);
							return NULL;
						}
						found_module = TRUE;
					}
					dir_list = dir_list->fdls_next;
				}
				if (!found_module)
				{
					gds__free(mod);
					return NULL;
				}
			}					/* else module is not in the INTL directory */
		}						/* else module is not in the default UDF directory, so ... */
	}							/* if dirname is "." */
	else
	{
		/*  The module name includes a directory path.
		 *  The directory must be the standard UDF directory, or the
		 *  standard international directory, or listed in
		 *  an EXTERNAL_FUNCTION_DIRECTORY line in isc_config,
		 *  and the module must be accessible in that directory.
		 */
		gds__prefix(ib_lib_path, IB_UDF_DIR);
		ib_lib_path[strlen(ib_lib_path) - 1] = '\0';	/* drop trailing "/" */
		found_module = !strcmp(ib_lib_path, dirp);
		if (!found_module)
		{
			gds__prefix(ib_lib_path, IB_INTL_DIR);
			ib_lib_path[strlen(ib_lib_path) - 1] = '\0';	/* drop trailing / */
			found_module = !strcmp(ib_lib_path, dirp);
		}
		if (!found_module)
		{
			/* It's not the default directory, so try the ones listed
			 * in EXTERNAL_FUNCTION_DIRECTORY lines in isc_config.
			 */
			dir_list = DLS_get_func_dirs();
			while (dir_list && !found_module)
			{
				if (!strcmp(dir_list->fdls_directory, dirp))
					found_module = TRUE;
				dir_list = dir_list->fdls_next;
			}
		}
		if (found_module)
			found_module = (!access(module, R_OK | X_OK)) &&
				(0 != (mod->mod_handle = shl_load(module, BIND_DEFERRED, 0)));
		if (!found_module)
		{
			gds__free(mod);
			return NULL;
		}
	}							/* else module name includes a directory path, so ... */
	return mod;
}
#endif


#ifdef DYNAMIC_SHARED_LIBRARIES
#define LOOKUP
FPTR_INT ISC_lookup_entrypoint(TEXT* module,
							   TEXT* name,
							   TEXT* ib_path_env_var)
{
/**************************************
 *
 *	I S C _ l o o k u p _ e n t r y p o i n t  ( SVR4 including SOLARIS )
 *
 **************************************
 *
 * Functional description
 *	Lookup entrypoint of function.
 *
 **************************************/
	FPTR_INT function;

	function = FUNCTIONS_entrypoint(module, name);
	if (function)
	{
		return function;
	}

#ifdef NON_DL_COMPATIBLE
	return NULL;
#else

	terminate_at_space(module);
	terminate_at_space(name);

	if (!*module || !*name)
	{
		return NULL;
	}

	// Check if external function module has already been loaded
	MOD mod = FLU_lookup_module(module);

	if (!mod)
	{
		TEXT absolute_module[MAXPATHLEN];

#ifdef EXT_LIB_PATH
		if (ib_path_env_var == NULL)
		{
			strcpy(absolute_module, module);
		}
		else
		{
			if (!gds__validate_lib_path(module,
										ib_path_env_var,
										absolute_module,
										sizeof(absolute_module)))
			{
				return NULL;
			}
		}
#else
		strcpy(absolute_module, module);
#endif /* EXT_LIB_PATH */

		const USHORT length = strlen(absolute_module);

		/* call search_for_module with the supplied name,
		   and if unsuccessful, then with <name>.so . */
		mod = search_for_module(absolute_module, name);

		if (!mod) {
			strcat(absolute_module, ".so");
			mod = search_for_module(absolute_module, name);
		}

        if (!mod) {  // Start looking for "libxxxx.so" module names
            string moduleName = "lib";
            moduleName += (const char*) absolute_module;
			mod = search_for_module((TEXT*) moduleName.c_str(), name);
        }

		if (!mod) {
			return NULL;
		}

		assert(mod->mod_handle);	/* assert that we found the module */
		mod->mod_use_count = 0;
		mod->mod_length = length;
		strcpy(mod->mod_name, module);
		mod->mod_next = FLU_modules;
		FLU_modules = mod;
	}

	++mod->mod_use_count;

	return (FPTR_INT) dlsym(mod->mod_handle, name);
#endif /* NON_DL_COMPATIBLE */
}


static MOD search_for_module(TEXT * module, TEXT * name)
{
/**************************************
 *
 *	s e a r c h _ f o r _ m o d u l e	( SVR4 including SOLARIS )
 *
 **************************************
 *
 * Functional description
 *	Look for a module (as named in a 'DECLARE EXTERNAL FUNCTION'
 *      statement.
 *
 **************************************/
	TEXT ib_lib_path[MAXPATHLEN];
	TEXT absolute_module[MAXPATHLEN];	/* for _access()  ???   */
	FDLS *dir_list;
	BOOLEAN found_module;

	strcpy(absolute_module, module);

	MOD mod = (MOD) gds__alloc(sizeof(struct mod) + strlen(absolute_module));

	if (!mod)
	{
		return NULL;
	}

	char* dirp = dirname(absolute_module);

	if (('.' == dirp[0]) && ('\0' == dirp[1]))
	{
		/*  We have a simple module name without a directory. */

		gds__prefix(ib_lib_path, IB_UDF_DIR);
		strncat(ib_lib_path, module, MAXPATHLEN - strlen(ib_lib_path) - 1);
		if (!access(ib_lib_path, R_OK))
		{
			/* Module is in the standard UDF directory: load it. */
			if (!(mod->mod_handle = dlopen(ib_lib_path, RTLD_LAZY)))
			{
#ifdef DEV_BUILD
			    gds__log("%s: %s\n", ib_lib_path, dlerror());
#endif
				gds__free(mod);
				return NULL;
			}
		}
		else
		{
			gds__prefix(ib_lib_path, IB_INTL_DIR);
			strncat(ib_lib_path, module,
					MAXPATHLEN - strlen(ib_lib_path) - 1);
			if (!access(ib_lib_path, R_OK))
			{
				/* Module is in the default directory: load it. */
				mod->mod_handle = dlopen(ib_lib_path, RTLD_LAZY);
				if (!mod->mod_handle)
				{
#ifdef DEV_BUILD
					gds__log("%s: %s\n", ib_lib_path, dlerror());			
#endif
					gds__free(mod);
					return NULL;
				}
			}
			else
			{
				/*  The module is not in the default directory, so ...
				 *  use the EXTERNAL_FUNCTION_DIRECTORY lines from isc_config.
				 */
				dir_list = DLS_get_func_dirs();
				found_module = FALSE;
				while (dir_list && !found_module)
				{
					strcpy(ib_lib_path, dir_list->fdls_directory);
					strcat(ib_lib_path, "/");
					strncat(ib_lib_path, module,
							MAXPATHLEN - strlen(ib_lib_path) - 1);
					if (!access(ib_lib_path, R_OK))
					{
						mod->mod_handle = dlopen(ib_lib_path, RTLD_LAZY);
						if (!mod->mod_handle)
						{
#ifdef DEV_BUILD
							gds__log("%s: %s\n", ib_lib_path, dlerror());							
#endif
							gds__free(mod);
							return NULL;
						}
						found_module = TRUE;
					}
					dir_list = dir_list->fdls_next;
				}
				if (!found_module)
				{
#ifdef DEV_BUILD
					gds__log("%s: not in a valid UDF directory\n", module);
#endif
					gds__free(mod);
					return NULL;
				}
			}					/* else module is not in the INTL directory */
		}						/* else module is not in the default directory, so ... */
	}							/* if *dirp is "." */
	else
	{
		/*  The module name includes a directory path.
		 *  The directory must be the standard UDF directory, or the
		 *  standard international directory, or listed in
		 *  an EXTERNAL_FUNCTION_DIRECTORY line in isc_config,
		 *  and the module must be accessible in that directory.
		 */
		gds__prefix(ib_lib_path, IB_UDF_DIR);
		ib_lib_path[strlen(ib_lib_path) - 1] = '\0';	/* drop trailing "/" */

		found_module = !strcmp(ib_lib_path, dirp);
		if (!found_module)
		{
			gds__prefix(ib_lib_path, IB_INTL_DIR);
			ib_lib_path[strlen(ib_lib_path) - 1] = '\0';	/* drop trailing / */
			found_module = !strcmp(ib_lib_path, dirp);
		}
		if (!found_module)
		{
			/* It's not the default directory, so try the ones listed
			 * in EXTERNAL_FUNCTION_DIRECTORY lines in isc_config.
			 */
			dir_list = DLS_get_func_dirs();
			while (dir_list && !found_module)
			{
				if (!strcmp(dir_list->fdls_directory, dirp))
					found_module = TRUE;
				dir_list = dir_list->fdls_next;
			}
		}
		if (found_module)
		{
		    if (!access(module, R_OK)) 
			{
				if (!(mod->mod_handle = dlopen(module, RTLD_LAZY)))
				{
#ifdef DEV_BUILD
					gds__log("%s: %s\n", module, dlerror());
#endif
					gds__free(mod);
					return NULL;						
				}
			}
			else
			{
#ifdef DEV_BUILD
			    gds__log("%s: file access error\n", module);
#endif
			    gds__free(mod);
				return NULL;
			}
		} /* else module name includes a directory path, so ... */
	    else 
		{
#ifdef DEV_BUILD
			gds__log("%s: not found in a valid UDF directory \n", module);
#endif
			gds__free(mod);
			return NULL;
		}
    }
	return mod;
}
#endif


#ifdef WIN_NT
#define LOOKUP
FPTR_INT ISC_lookup_entrypoint(TEXT* module,
							   TEXT* name,
							   TEXT* ib_path_env_var)
{
/**************************************
 *
 *	I S C _ l o o k u p _ e n t r y p o i n t	( W I N _ N T )
 *
 **************************************
 *
 * Functional description
 *	Lookup entrypoint of function.
 *	If ib_path_env_var is defined, make sure
 *	that the module is in the path defined
 *	by that environment variable.
 *
 **************************************/
	FPTR_INT function;

	function = FUNCTIONS_entrypoint(module, name);
	if (function)
	{
		return function;
	}

	terminate_at_space(name);

	// Check if external function module has already been loaded
	MOD mod = FLU_lookup_module(module);

	if (!mod)
	{
		TEXT absolute_module[MAXPATHLEN];	// for _access()

#ifdef EXT_LIB_PATH
		if (ib_path_env_var == NULL)
		{
			strcpy(absolute_module, module);
		}
		else
		{
			if (!gds__validate_lib_path(module,
										ib_path_env_var,
										absolute_module,
										sizeof(absolute_module)))
			{
				return NULL;
			}
		}
#else
		strcpy(absolute_module, module);
#endif /* EXT_LIB_PATH */

		const USHORT length = strlen(absolute_module);

		/* call search_for_module with the supplied name, and if unsuccessful,
		   then with <name>.DLL (if the name did not have a trailing "."). */
		mod = search_for_module(absolute_module, name);
		if (!mod)
		{
			if ((absolute_module[length - 1] != '.') &&
				stricmp(absolute_module + length - 4, ".DLL"))
			{
				strcat(absolute_module, ".DLL");
				mod = search_for_module(absolute_module, name);
			}
		}
		if (!mod)
		{
			return NULL;
		}

		assert(mod->mod_handle);	/* assert that we found the module */
		mod->mod_use_count = 0;
		mod->mod_length = length;
		strcpy(mod->mod_name, module);
		mod->mod_next = FLU_modules;
		FLU_modules = mod;
	}

	++mod->mod_use_count;

	// The Borland compiler prefixes an '_' for functions compiled
	// with the __cdecl calling convention.
	// TMN: Doesn't all_ ompilers do that?

	function = (FPTR_INT) GetProcAddress(mod->mod_handle, name);
	if (!function)
	{
		TEXT buffer[128];
		buffer[0] = '_';
		strcpy(&buffer[1], name);
		function = (FPTR_INT) GetProcAddress(mod->mod_handle, buffer);
	}

	return function;
}


static MOD search_for_module(TEXT* module, TEXT* name)
{
/**************************************
 *
 *	s e a r c h _ f o r _ m o d u l e	( W I N _ N T )
 *
 **************************************
 *
 * Functional description
 *	Look for a module (as named in a 'DECLARE EXTERNAL FUNCTION'
 *      statement.  Adjust for differences between behavior of
 *      _access() and LoadLibrary() w.r.t. implicit .DLL suffix.
 *
 **************************************/

 /*  We need different versions of the file name to pass to _access()
 *  and to LoadLibrary(), because LoadLibrary() implicitly appends
 *  .DLL to the supplied name and access does not.  We need to control
 *  exactly what name is being used by each routine.
 */
	TEXT ib_lib_path[MAXPATHLEN];	/* for access()      */
	TEXT absolute_module[MAXPATHLEN];	/* for _access()  ???   */
	TEXT loadlib_name[MAXPATHLEN];	/* for LoadLibrary() */
	TEXT drive_buf[_MAX_DRIVE];
	TEXT dir_buf[_MAX_DIR];
	FDLS *dir_list;
	BOOLEAN found_module;

	const USHORT length = strlen(module);

	MOD mod = (MOD) gds__alloc(sizeof(struct mod) + length);

	if (!mod)
	{
		return NULL;
	}

	/* Set absolute_module to the drive and directory prefix part of
	   module, like dirname() under Unix. */
	_splitpath(module, drive_buf, dir_buf, 0, 0);
	strcpy(absolute_module, drive_buf);
	strcat(absolute_module, dir_buf);

	if (0 == absolute_module[0])
	{
		/*  We have a simple module name without a directory. */

		gds__prefix(ib_lib_path, IB_UDF_DIR);
		strncat(ib_lib_path, module, MAXPATHLEN - strlen(ib_lib_path) - 1);
		adjust_loadlib_name(ib_lib_path, loadlib_name);
		if (!_access(ib_lib_path, 4 /* read permission */ ))
		{
			/* Module is in the default UDF directory: load it. */
			mod->mod_handle = LoadLibrary(loadlib_name);
			if (!mod->mod_handle)
			{
				gds__free(mod);
				return NULL;
			}
		}
		else
		{
			gds__prefix(ib_lib_path, IB_INTL_DIR);
			strncat(ib_lib_path, module,
					MAXPATHLEN - strlen(ib_lib_path) - 1);
			if (!_access(ib_lib_path, 4 /* read permission */ ))
			{
				/* Module is in the default international library: load it */
				mod->mod_handle = LoadLibrary(ib_lib_path);
				if (!mod->mod_handle)
				{
					gds__free(mod);
					return NULL;
				}
			}
			else
			{
				/*  The module is not in the default directory, so ...
				 *  use the EXTERNAL_FUNCTION_DIRECTORY lines from isc_config.
				 */
				dir_list = DLS_get_func_dirs();
				found_module = FALSE;
				while (dir_list && !found_module)
				{
					strcpy(ib_lib_path, dir_list->fdls_directory);
					strcat(ib_lib_path, "\\");
					strncat(ib_lib_path, module,
							MAXPATHLEN - strlen(ib_lib_path) - 1);
					if (!_access(ib_lib_path, 4 /* read permission */ ))
					{
						mod->mod_handle = LoadLibrary(ib_lib_path);
						if (!mod->mod_handle)
						{
							gds__free(mod);
							return NULL;
						}
						found_module = TRUE;
					}
					dir_list = dir_list->fdls_next;
				}
				if (!found_module)
				{
					gds__free(mod);
					return NULL;
				}
			}					/* else module is not in the INTL directory */
		}						/* else module is not in the default UDF directory, so ... */
	}							/* if path part of name is empty */
	else
	{
		/*  The module name includes a directory path.
		 *  The directory must be either $INTERBASE/UDF, or $INTERBASE/intl
		 *  or listed in an EXTERNAL_FUNCTION_DIRECTORY line in isc_config,
		 *  and the module must be accessible in that directory.
		 */
		gds__prefix(ib_lib_path, IB_UDF_DIR);
		found_module = !stricmp(ib_lib_path, absolute_module);
		if (!found_module)
		{
			gds__prefix(ib_lib_path, IB_INTL_DIR);
			found_module = !stricmp(ib_lib_path, absolute_module);
		}
		if (!found_module)
		{
			/* It's not the default directory, so try the ones listed
			 * in EXTERNAL_FUNCTION_DIRECTORY lines in isc_config.
			 */
			dir_list = DLS_get_func_dirs();
			while (dir_list && !found_module)
			{
				if (!stricmp(dir_list->fdls_directory, absolute_module))
					found_module = TRUE;
				dir_list = dir_list->fdls_next;
			}
		}
		if (found_module)
		{
			found_module =
				(!_access(module, 4 /* read ok? */ )) &&
				(0 != (mod->mod_handle = LoadLibrary(module)));
		}
		if (!found_module)
		{
			gds__free(mod);
			return NULL;
		}
	}							/* else module name includes a directory path, so ... */
	return mod;
}


static void adjust_loadlib_name(TEXT * access_path, TEXT * load_path)
{
/**************************************
 *
 *	a d j u s t _ l o a d l i b _ n a m e	( W I N _ N T )
 *
 **************************************
 *
 * Functional description
 *	Look for a module (as named in a 'DECLARE EXTERNAL FUNCTION'
 *      statement.  Adjust for differences between behavior of
 *      _access() and LoadLibrary() w.r.t. implicit .DLL suffix.
 *
 **************************************/

	const USHORT length = strlen(access_path);
	strcpy(load_path, access_path);

	/*  Adjust the names to be passed to _access() and LoadLibrary(),
	 *  so that they will refer to the same file, despite the implicit
	 *  adjustments performed by LoadLibrary.
	 *  Input     _access() arg   LoadLibrary() arg  LoadLibrary() uses...
	 *  -------   -------------   -----------------  ------------------
	 *  foo.      foo             foo.               foo
	 *  foo       foo             foo.               foo
	 *  foo.dll   foo.dll         foo.dll            foo.dll
	 */
	if (stricmp(access_path + length - 4, ".dll"))
	{
		if ('.' == access_path[length - 1])
		{
			access_path[length - 1] = '\0';
		}
		else
		{
			load_path[length] = '.';
			load_path[length + 1] = '\0';
		}
	}
}
#endif

#ifdef DARWIN
#define LOOKUP
NSModule ISC_link_with_module (TEXT*);

FPTR_INT ISC_lookup_entrypoint (
    TEXT        *module,
    TEXT        *name,
    TEXT        *ib_path_env_var)
{
/**************************************
 *
 *      I S C _ l o o k u p _ e n t r y p o i n t  ( D A R W I N )
 *
 **************************************
 *
 * Functional description
 *      Lookup entrypoint of function.
 *
 **************************************/
FPTR_INT        function;
int             lastSpace, i, len;
MOD             mod;
TEXT            absolute_module[MAXPATHLEN];
NSSymbol        symbol;
if (function = FUNCTIONS_entrypoint (module, name))
    return function;

/* Remove TRAILING spaces from path names; spaces within the path are valid */
for (i = strlen(module) - 1, lastSpace = 0; i >= 0; i--)
    if (module[i] == ' ')
        lastSpace = i;
    else
        break;
if (module[lastSpace] == ' ')
    module[lastSpace] = 0;

for (i = strlen(name) - 1, lastSpace = 0; i >= 0; i--)
    if (name[i] == ' ')
        lastSpace = i;
    else
        break;
if (name[lastSpace] == ' ')
    name[lastSpace] = 0;

if (!*module || !*name)
    return NULL;

/*printf("names truncated: %s, function %s.\n", module, name);*/
/* Check if external function module has already been loaded */
if (!(mod = FLU_lookup_module (module)))
    {
    USHORT length ;

#ifdef EXT_LIB_PATH
    if (ib_path_env_var == NULL)
        strcpy (absolute_module, module);
    else if (!gds__validate_lib_path (module, ib_path_env_var, absolute_module, sizeof(absolute_module)))
        return NULL;
#else
    strcpy (absolute_module, module);
#endif /* EXT_LIB_PATH */

    length = strlen (absolute_module);
    /* call search_for_module with the supplied name,
       and if unsuccessful, then with <name>.so . */
    mod = search_for_module (absolute_module, name);
    if (!mod)
        {
        strcat (absolute_module, ".so");
        mod = search_for_module (absolute_module, name);
        }
    if (!mod)
    {
        /*printf("Couldn't find module: %s\n", absolute_module);*/
        return NULL;
    }

    assert (mod->mod_handle);   /* assert that we found the module */
    mod->mod_use_count = 0;
    mod->mod_length = length;
    strcpy (mod->mod_name, module);
    mod->mod_next = FLU_modules;
    FLU_modules = mod;
    }

/* Look for the symbol and return a pointer to the function if found */
mod->mod_use_count++;
symbol = NSLookupSymbolInModule(mod->mod_handle, name);
if (symbol == NULL)
{
    /*printf("Failed to find function: %s in module %s, trying _%s\n", name, module, name);*/
    strcpy(absolute_module, "_");
    strncat(absolute_module, name, sizeof(absolute_module) - 2);
    symbol = NSLookupSymbolInModule(mod->mod_handle, absolute_module);
    if (symbol == NULL)
    {
        /*printf("Failed to find symbol %s.  Giving up.\n", absolute_module);*/
        return NULL;
    }
}
return (FPTR_INT) NSAddressOfSymbol(symbol);

}

static MOD search_for_module ( TEXT *module,
                               TEXT *name )
{
/**************************************
 *
 *      s e a r c h _ f o r _ m o d u l e       ( D A R W I N )
 *
 **************************************
 *
 * Functional description
 *      Look for a module (as named in a 'DECLARE EXTERNAL FUNCTION'
 *      statement.
 *
 **************************************/
MOD             mod;
char           *dirp;
TEXT            ib_lib_path[MAXPATHLEN];
TEXT            absolute_module[MAXPATHLEN];  /* for _access()  ???   */
FDLS            *dir_list;
BOOLEAN         found_module;

strcpy (absolute_module, module);

if (!(mod = (MOD) gds__alloc (sizeof (struct mod) +
                              strlen (absolute_module))))
    return NULL;
dirp = (char*) dirname (absolute_module);
if (('.' == dirp[0]) && ('\0' == dirp[1]))
    {
    /*  We have a simple module name without a directory. */

    gds__prefix (ib_lib_path, IB_UDF_DIR);
    strncat (ib_lib_path, module,
             MAXPATHLEN - strlen (ib_lib_path) - 1);
    if (!access (ib_lib_path, R_OK))
        {
        /* Module is in the standard UDF directory: load it. */
        if (!(mod->mod_handle = ISC_link_with_module (ib_lib_path)))
            {
            gds__free (mod);
            return NULL;
            }
        }
    else
        {
        gds__prefix (ib_lib_path, IB_INTL_DIR);
        strncat (ib_lib_path, module,
                 MAXPATHLEN - strlen (ib_lib_path) - 1);
        /*printf("intl file: %s\n", ib_lib_path);*/
        if (!access (ib_lib_path, R_OK))
            {
            /* Module is in the default directory: load it. */
            if (!(mod->mod_handle = ISC_link_with_module (ib_lib_path)))
                {
                gds__free (mod);
                return NULL;
                }
            }
        else
            {
            /*  The module is not in the default directory, so ...
             *  use the EXTERNAL_FUNCTION_DIRECTORY lines from isc_config.
             */
            dir_list = DLS_get_func_dirs();
            found_module = FALSE;
            while (dir_list && !found_module)
                {
                strcpy (ib_lib_path, dir_list->fdls_directory);
                strcat (ib_lib_path, "/");
                strncat (ib_lib_path, module,
                         MAXPATHLEN - strlen (ib_lib_path) - 1);
                if (!access (ib_lib_path, R_OK))
                    {
                    if (!(mod->mod_handle = ISC_link_with_module (ib_lib_path)))
                        {
                        gds__free (mod);
                        return NULL;
                        }
                    found_module = TRUE;
                    }
                dir_list = dir_list->fdls_next;
                }
            if (!found_module)
                {
                gds__free (mod);
                return NULL;
                }
            } /* else module is not in the INTL directory */
        } /* else module is not in the default directory, so ... */
    } /* if *dirp is "." */
else
    {
    /*  The module name includes a directory path.
     *  The directory must be the standard UDF directory, or the
     *  standard international directory, or listed in
     *  an EXTERNAL_FUNCTION_DIRECTORY line in isc_config,
     *  and the module must be accessible in that directory.
     */
    gds__prefix (ib_lib_path, IB_UDF_DIR);
    ib_lib_path [strlen(ib_lib_path) - 1] = '\0'; /* drop trailing "/" */
    found_module = ! strcmp (ib_lib_path, dirp);
    if (!found_module)
        {
        gds__prefix (ib_lib_path, IB_INTL_DIR);
        ib_lib_path [strlen(ib_lib_path) - 1] = '\0'; /* drop trailing / */
        found_module = ! strcmp (ib_lib_path, dirp);
        }
    if (!found_module)
        {
        /* It's not the default directory, so try the ones listed
         * in EXTERNAL_FUNCTION_DIRECTORY lines in isc_config.
         */
        dir_list = DLS_get_func_dirs();
        while (dir_list && !found_module)
            {
            if (! strcmp (dir_list->fdls_directory, dirp))
              found_module = TRUE;
            dir_list = dir_list->fdls_next;
            }
        }
    if (found_module)
        found_module = (!access (module, R_OK)) &&
          (0 !=  (mod->mod_handle = ISC_link_with_module (module)));
    if (!found_module)
        {
        gds__free (mod);
        return NULL;
        }
    } /* else module name includes a directory path, so ... */
return mod;
}

NSModule ISC_link_with_module (
    TEXT        *fileName)
{
/**************************************
 *
 *      I S C _ l i n k _ w i t h _ m o d u l e  ( D A R W I N )
 *
 **************************************
 *
 * Functional description
 *      Given the file system path to a module, load it and link it into
 *         our address space.  Assumes all security checks have been
 *         performed.
 *
 **************************************/
 NSObjectFileImage image;
 NSObjectFileImageReturnCode retVal;
 NSModule mod_handle;
 NSSymbol initSym;
 void (*init)(void);

 /* Create an object file image from the given path */
 retVal = NSCreateObjectFileImageFromFile(fileName, &image);
 if(retVal != NSObjectFileImageSuccess)
 {
     switch(retVal)
     {
         case NSObjectFileImageFailure:
                /*printf("object file setup failure");*/
                return NULL;
            case NSObjectFileImageInappropriateFile:
                /*printf("not a Mach-O MH_BUNDLE file type");*/
                return NULL;
            case NSObjectFileImageArch:
                /*printf("no object for this architecture");*/
                return NULL;
            case NSObjectFileImageFormat:
                /*printf("bad object file format");*/
                return NULL;
            case NSObjectFileImageAccess:
                /*printf("can't read object file");*/
                return NULL;
            default:
                /*printf("unknown error from NSCreateObjectFileImageFromFile()");*/
                return NULL;
      }
 }

 /* link the image */
 mod_handle = NSLinkModule(image, fileName, NSLINKMODULE_OPTION_PRIVATE);
 NSDestroyObjectFileImage(image) ;
 if(mod_handle == NULL)
 {
     /*printf("NSLinkModule() failed for dlopen()");*/
     return NULL;
 }

 initSym = NSLookupSymbolInModule(mod_handle, "__init");
 if (initSym != NULL)
 {
     init = ( void (*)(void)) NSAddressOfSymbol(initSym);
     init();
 }

 return mod_handle;
}

#endif


#ifndef LOOKUP
FPTR_INT ISC_lookup_entrypoint(TEXT* module,
							   TEXT* name,
							   TEXT* ib_path_env_var)
{
/**************************************
 *
 *	I S C _ l o o k u p _ e n t r y p o i n t  ( d e f a u l t )
 *
 **************************************
 *
 * Functional description
 *	Lookup entrypoint of function.
 *
 **************************************/

	FPTR_INT function = FUNCTIONS_entrypoint(module, name);
	return function;
}
#endif


#ifdef VMS
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


} // extern "C"
