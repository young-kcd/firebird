/*
 *	PROGRAM:	InterBase Access Method
 *	MODULE:		functions.c
 *	DESCRIPTION:	External entrypoint definitions
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
 */

#include "firebird.h"
#include "../jrd/ib_stdio.h"
#include <string.h>
#include "../jrd/jrd.h"  /* For MAXPATHLEN Bug #126614 */

/* defined in common.h, which is included by ib_stdio.h: typedef int (*FPTR_INT)(); */


extern "C" {


typedef struct {
	char *fn_module;
	char *fn_entrypoint;
	FPTR_INT fn_function;
} FN;


FPTR_INT FUNCTIONS_entrypoint(char *, char *);
static int test(long, char *);

static DSC* ni(DSC*, DSC*);


#pragma FB_COMPILER_MESSAGE("Fix! function pointer cast!")

static FN isc_functions[] = {
	{"test_module", "test_function", (int (*)()) test},
	{"test_module", "ni", (int (*)()) ni},
	{"test_module", "ns", (int (*)()) ni},
	{"test_module", "nn", (int (*)()) ni},
	{0, 0, 0}
};


FPTR_INT FUNCTIONS_entrypoint(char *module, char *entrypoint)
{
/**************************************
 *
 *	F U N C T I O N S _ e n t r y p o i n t
 *
 **************************************
 *
 * Functional description
 *	Lookup function in hardcoded table.  The module and
 *	entrypoint names are null terminated, but may contain
 *	insignificant trailing blanks.
 *
 **************************************/
	FN *function;
	char *p, temp[MAXPATHLEN + 128], *ep;  /* Bug #126614 Fix */

	p = temp;

	while (*module && *module != ' ')
		*p++ = *module++;

	*p++ = 0;
	ep = p;

	while (*entrypoint && *entrypoint != ' ')
		*p++ = *entrypoint++;

	*p = 0;

	for (function = isc_functions; function->fn_module; ++function)
		if (!strcmp(temp, function->fn_module)
			&& !strcmp(ep, function->fn_entrypoint))
			return function->fn_function;

	return 0;
}


static int test(long n, char *result)
{
/**************************************
 *
 *	t e s t
 *
 **************************************
 *
 * Functional description
 *	Sample extern function.  Defined in database by:
 *
 *	define function test module_name "test_module" entry_point "test_function"
 *	    long by value,
 *	    char [20] by reference return_argument;
 *
 **************************************/

	sprintf(result, "%ld is a number", n);
	const char *end = result + 20;

	while (*result)
		result++;

	while (result < end)
		*result++ = ' ';

	return 0;
}


static DSC* ni(DSC* v, DSC* v2)
{
	if (v)
		return v;
	else
		return v2;
}

} // extern "C"
