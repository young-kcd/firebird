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
 * 2003.04.12 Alex Peshkoff - Security code cleanup:
 *		1.	Drop EXT_LIB_PATH verification
 *		2.	Drop IB_UDF_DIR & IB_INTL_DIR support
 *		3.	Created common for all platforms search_for_module,
 *			using dir_list and PathUtils. Platform specific
 *			macros defined after ISC_lookup_entrypoint()
 *			for each platform.
 *
 */
/*
$Id: flu.cpp,v 1.35 2003-09-22 17:52:27 brodsom Exp $
*/

#include "firebird.h"
#include "../common/config/config.h"
#include "../common/config/dir_list.h"
#include "../jrd/os/path_utils.h"

#include <string>

using namespace std;

#include "../jrd/common.h"
#include "../jrd/flu.h"
#include "../jrd/gdsassert.h"

#include "../jrd/flu_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/dls_proto.h"
#include "../jrd/err_proto.h"

#include "../include/gen/codes.h"

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
#endif

#if (defined SOLARIS || defined SCO_EV || defined LINUX || defined AIX_PPC || defined SINIXZ || defined FREEBSD)
#include <dlfcn.h>
#define DYNAMIC_SHARED_LIBRARIES
#include <unistd.h>
#include <libgen.h>
#endif

#ifdef DARWIN
#include <unistd.h>
#include <mach-o/dyld.h>
#include <libgen.h>
#endif

#if defined NETBSD
#include <dlfcn.h>
#define DYNAMIC_SHARED_LIBRARIES
#include <unistd.h>
#endif

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
#endif


extern "C" {

static MOD FLU_modules = 0;		/* External function/filter modules */

/* prototypes for private functions */
static MOD search_for_module(TEXT *, TEXT *, bool);


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
							   TEXT * name, TEXT * ib_path_env_var,
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
							   TEXT* ib_path_env_var,
							   bool ShowAccessError)
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
							   TEXT * name, TEXT * ib_path_env_var,
							   bool ShowAccessError)
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
		strcpy(absolute_module, module);

		const USHORT length = strlen(absolute_module);

		/* call search_for_module with the supplied name,
		   and if unsuccessful, then with <name>.sl . */
		mod = search_for_module(absolute_module, name, false);
		if (!mod)
		{
			strcat(absolute_module, ".sl");
			mod = search_for_module(absolute_module, name, ShowAccessError);
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

#define REQUIRED_MODULE_ACCESS (R_OK | X_OK)
#define OPEN_HANDLE(name) shl_load(name.c_str(), BIND_DEFERRED, 0)

#endif


#ifdef DYNAMIC_SHARED_LIBRARIES
#define LOOKUP
FPTR_INT ISC_lookup_entrypoint(TEXT* module,
							   TEXT* name,
							   TEXT* ib_path_env_var,
							   bool ShowAccessError)
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
		strcpy(absolute_module, module);

		const USHORT length = strlen(absolute_module);

		/* call search_for_module with the supplied name,
		   and if unsuccessful, then with <name>.so . */
		mod = search_for_module(absolute_module, name, false);

		if (!mod) {
			strcat(absolute_module, ".so");
			mod = search_for_module(absolute_module, name, false);
		}

        if (!mod) {  // Start looking for "libxxxx.so" module names
            string moduleName = "lib";
            moduleName += (const char*) absolute_module;
			mod = search_for_module((TEXT*) moduleName.c_str(), name, ShowAccessError);
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

#define REQUIRED_MODULE_ACCESS (R_OK)
#define OPEN_HANDLE(name) dlopen(name.c_str(), RTLD_LAZY)
 
#endif


#ifdef WIN_NT
#define LOOKUP
FPTR_INT ISC_lookup_entrypoint(TEXT* module,
							   TEXT* name,
							   TEXT* ib_path_env_var,
							   bool ShowAccessError)
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
		strcpy(absolute_module, module);

		const USHORT length = strlen(absolute_module);

		/* call search_for_module with the supplied name, and if unsuccessful,
		   then with <name>.DLL (if the name did not have a trailing "."). */
		mod = search_for_module(absolute_module, name, false);
		if (!mod)
		{
			if ((absolute_module[length - 1] != '.') &&
				stricmp(absolute_module + length - 4, ".DLL"))
			{
				strcat(absolute_module, ".DLL");
				mod = search_for_module(absolute_module, name, ShowAccessError);
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


#define REQUIRED_MODULE_ACCESS 4
#define OPEN_HANDLE(name) AdjustAndLoad(name)

HMOD AdjustAndLoad(Firebird::string name) {
/**************************************
 *
 *	A d j u s t A n d L o a d	( W I N _ N T )
 *
 **************************************
 *
 * Functional description
 *	Before calling LoadLibrary(), add dot to the end of module
 *		name, to provide stable behaviour (implicit .DLL suffix).
 *
 **************************************/
	Firebird::string suffix = name.length() > 4 ?
		name.substr(name.length() - 4, 4) : Firebird::string("");
	if (stricmp(suffix.c_str(), ".dll") != 0) {
		if (name.substr(name.length() - 1, 1) != ".") {
			name += '.';
		}
	}
	return LoadLibrary(name.c_str());
}
#endif

#ifdef DARWIN
#define LOOKUP
NSModule ISC_link_with_module (TEXT*);

FPTR_INT ISC_lookup_entrypoint (
    TEXT        *module,
    TEXT        *name,
    TEXT        *ib_path_env_var,
	bool		ShowAccessError)
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
    strcpy (absolute_module, module);
    length = strlen (absolute_module);
    /* call search_for_module with the supplied name,
       and if unsuccessful, then with <name>.so . */
    mod = search_for_module (absolute_module, name, false);
    if (!mod)
        {
        strcat (absolute_module, ".so");
        mod = search_for_module (absolute_module, name, ShowAccessError);
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

#define REQUIRED_MODULE_ACCESS (R_OK)
#define OPEN_HANDLE(name) ISC_link_with_module(name.c_str())

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
							   TEXT* ib_path_env_var,
							   bool ShowAccessError)
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

static MOD search_for_module(TEXT* module, TEXT* name, bool ShowAccessError)
{
/**************************************
 *
 *	s e a r c h _ f o r _ m o d u l e
 *
 **************************************
 *
 * Functional description
 *	Look for a module (as named in a 'DECLARE EXTERNAL FUNCTION'
 *      statement.
 *
 **************************************/
#ifndef REQUIRED_MODULE_ACCESS
	return NULL;
#else
	static class UdfDirectoryList : public DirectoryList {
		const Firebird::PathName GetConfigString(void) const {
			return Firebird::PathName(Config::getUdfAccess());
		}
	} iUdfDirectoryList;

	Firebird::PathName path, relative;
	Firebird::PathName absolute_module = module;

	// Search for module name in UdfAccess restricted paths list
	PathUtils::splitLastComponent(path, relative, absolute_module);
	if (path.length() == 0 && PathUtils::isRelative(absolute_module)) {
		relative = absolute_module;
		iUdfDirectoryList.ExpandFileName(absolute_module, relative, REQUIRED_MODULE_ACCESS);
	}

	// The module name, including directory path,
	// must satisfy UdfAccess entry in config file.
	if (!iUdfDirectoryList.IsPathInList(absolute_module)) {
		if (ShowAccessError) {
			ERR_post(gds_conf_access_denied,
				gds_arg_string, "UDF library",
				gds_arg_string, ERR_cstring(const_cast <TEXT *>(absolute_module.c_str())),
				gds_arg_end);
		}
		return NULL;
	}

	MOD mod = (MOD) gds__alloc(sizeof(struct mod) + absolute_module.length());
	if (!mod) {
		return NULL;
	}

	if (!(mod->mod_handle = OPEN_HANDLE(absolute_module))) {
/*
 * Temporarily commented - what to do with dlerror() on NT ?
#ifdef DEV_BUILD
		gds__log("%s: %s\n", module, dlerror());
#endif
 */
		gds__free(mod);
		return NULL;						
	}
	return mod;
#endif  //REQUIRED_MODULE_ACCESS
}

} // extern "C"
