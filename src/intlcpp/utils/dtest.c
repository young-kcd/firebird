/*
 *	Test tool for Language Drivers.
 *
 *	This tool loads up a language driver using the dynamic link
 *	(or shared object) method and interrogates it's ID function.
 *	This tool is used to quickly verify the dynamic load ability
 *	of a newly created language driver.
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
 */

/*  define LIKE_JRD to have the lookup follow exactly the syntax
 *  used by intl.c in JRD.
 *  Set the static full_debug to 1 to turn on ib_printf debugging.
 *
 * 2002.02.15 Sean Leyne - Code Cleanup, removed obsolete "Apollo" port
 *
 */

#include "firebird.h"
#include "../jrd/ib_stdio.h"
#define DEBUG

static int full_debug = 0;
#define	FULL_DEBUG	if (full_debug) ib_printf



typedef unsigned short SHORT;

#include "../jrd/intl.h"

/* Following defines are duplicates of those in intl.c */
/* Name of module that implements text-type (n) */

#ifdef VMS
#define	INTL_MODULE "[syslib]IBLD_%03d"
#endif

#ifndef INTL_MODULE
#define	INTL_MODULE "lib/IBLD_%03d"
#endif

#ifndef INTL_INIT_ENTRY
#define INTL_INIT_ENTRY "ld_init"
#endif


#ifdef VMS
char *defaults[] = {
	"<null>",
	"IBLD_010",
	"IBLD_011", "ask", "ask", "ask", "ask", "ask", "ask", "ask"
};
#endif


try(c, f)
	 char *c;
	 FPTR_INT f;
{
	unsigned char buffer[200];
	int res;
	int i;
	res = (*f) (strlen(c), c, sizeof(buffer), buffer);
	ib_printf("%s => ", c);
	for (i = 0; i < res; i++)
		ib_printf("%d ", buffer[i]);
	ib_printf("\n");
};

main(argc, argv)
	 int argc;
	 char *argv[];
{
	FPTR_INT func;
	char buffer[200];
	int i;
	struct texttype this_textobj;
	char **vector;

#ifdef VMS
	vector = defaults;
	argc = FB_NELEM(defaults);
#else
	if (argc <= 1) {
		ib_printf("usage: dtest Intl_module_name\n");
		return (1);
	};
	vector = argv;
#endif

	for (i = 1; i < argc; i++) {

#ifdef LIKE_JRD
		{
			char module[200];
			char path[MAXPATHLEN];
			char entry[200];
			int t_type;
			t_type = atoi(vector[i]);
			sprintf(module, INTL_MODULE, t_type);
			gds__prefix(path, module);
			sprintf(entry, INTL_INIT_ENTRY, t_type);
			ib_printf("path=%s entry=%s\n", path, entry);
			func = (FPTR_INT) ISC_lookup_entrypoint(path, entry, NULL);
		}
#else
		if (strcmp(vector[i], "ask") == 0) {
			gets(buffer);
			func = (FPTR_INT) ISC_lookup_entrypoint(buffer, "ld_init", NULL);
		}
		else
			func =
				(FPTR_INT) ISC_lookup_entrypoint(vector[i], "ld_init", NULL);
#endif
		if (func == NULL)
			ib_printf("Cannot find %s.init\n", vector[i]);
		else {
			FULL_DEBUG("This testobj %ld\n", &this_textobj);
			FULL_DEBUG("size of %d\n", sizeof(this_textobj));
			if ((*func) (99, &this_textobj) != 99)
				ib_printf("%s.Init returned bad result\n", vector[i]);
			else {
				FULL_DEBUG("Called init ok\n");
				FULL_DEBUG("ld_init is %ld %ld\n",
						   this_textobj.
						   texttype_functions[(int) intl_fn_init], func);
				func = this_textobj.texttype_functions[intl_fn_NULL];
				FULL_DEBUG("ld_id is %ld %ld\n",
						   this_textobj.
						   texttype_functions[(int) intl_fn_NULL], func);
				if (func == NULL)
					ib_printf("%s.Init OK can't find ID\n", vector[i]);
				else {
					FULL_DEBUG("About to call ID fn\n");
					(*func) (sizeof(buffer), buffer);
					FULL_DEBUG("Back from ID fn \n");
					ib_printf("%s.id => %s\n", vector[i], buffer);
				};

				func = this_textobj.texttype_functions[intl_fn_string_to_key];

				FULL_DEBUG("ld_to_key is %ld\n", func);
				if (func == NULL)
					ib_printf("%s: Can't find str_to_key\n", vector[i]);
				else {
					try("cote", func);
					try("COTE", func);
					try("co-te", func);
					try("CO-TE", func);
				};
			};
		};
	};
	return (0);
}
