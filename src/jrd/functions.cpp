/*
 *	PROGRAM:	InterBase Access Method
 *	MODULE:		functions.cpp
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
#include "../jrd/common.h"
#include <stdio.h>
#include <string.h>
#include "../jrd/jrd.h"  /* For MAXPATHLEN Bug #126614 */
#include "../jrd/dsc_proto.h"

/* defined in common.h, which is included by stdio.h: typedef int (*FPTR_INT)(); */


extern "C" {


struct FN {
	const char* fn_module;
	const char* fn_entrypoint;
	FPTR_INT fn_function;
};


// FPTR_INT FUNCTIONS_entrypoint(char*, char*);
static int test(const long*, char*);
static DSC* ni(DSC*, DSC*);
static SLONG* byteLen(const dsc*);


#pragma FB_COMPILER_MESSAGE("Fix! function pointer cast!")

static const FN isc_functions[] = {
	{"test_module", "test_function", (FPTR_INT) test},
	{"test_module", "ni", (FPTR_INT) ni},
	{"test_module", "ns", (FPTR_INT) ni},
	{"test_module", "nn", (FPTR_INT) ni},
	{"test_module", "byte_len", (FPTR_INT) byteLen},
	{0, 0, 0}
};


FPTR_INT FUNCTIONS_entrypoint(const char* module, const char* entrypoint)
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
	char temp[MAXPATHLEN + 128];  /* Bug #126614 Fix */

	char* p = temp;

	while (*module && *module != ' ')
		*p++ = *module++;

	*p++ = 0;
	const char* const ep = p;

	while (*entrypoint && *entrypoint != ' ')
		*p++ = *entrypoint++;

	*p = 0;

	for (const FN* function = isc_functions; function->fn_module; ++function) {
		if (!strcmp(temp, function->fn_module)
			&& !strcmp(ep, function->fn_entrypoint))
		{
			return function->fn_function;
		}
	}

	return 0;
}


static int test(const long* n, char *result)
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
 *	QLI:
 *	define function test module_name "test_module" entry_point "test_function"
 *	    long by reference //by value,   CVC: BY VALUE is deprecated for input params
 *	    char [20] by reference return_argument;
 *	ISQL:
 *	declare external function test
 * 	int null, // CVC: with NULL signaling
 *	char(20) returns parameter 2
 *	entry_point 'test_function' module_name 'test_module';
 *
 **************************************/

	if (n)
		sprintf(result, "%ld is a number", *n);
	else
		sprintf(result, "is NULL");
	const char* const end = result + 20;

	while (*result)
		result++;

	while (result < end)
		*result++ = ' ';

	return 0;
}


static dsc* ni(dsc* v, dsc* v2)
{
	if (v)
		return v;
	else
		return v2;
}


// byteLen: return the length in bytes of a given argument. For NULL, return NULL, too.
// v = input descriptor
// rc = return value, allocated dynamically. To be freed by the engine.
// The declaration through SQL is:
// declare external function sys_byte_len
// int by descriptor
// returns int free_it
// entry_point 'byte_len' module_name 'test_module';
static SLONG* byteLen(const dsc* v)
{
	if (!v || !v->dsc_address || (v->dsc_flags & DSC_null))
		return 0;
	else
	{
		SLONG& rc = *(SLONG*) malloc(sizeof(SLONG));
		switch (v->dsc_dtype)
		{
		case dtype_text:
			{
				const UCHAR* const ini = v->dsc_address;
				const UCHAR* end = ini + v->dsc_length;
				while (ini < end && *--end == ' '); // empty loop body
				rc = end - ini + 1;
				break;
			}
		case dtype_cstring:
			{
				rc = 0;
				for (const UCHAR* p = v->dsc_address; *p; ++p, ++rc); // empty loop body
				break;
			}
		case dtype_varying:
			rc = reinterpret_cast<const vary*>(v->dsc_address)->vary_length;
			break;
		default:
			rc = DSC_string_length(v);
			break;
		}
		
		return &rc;
	}
}


} // extern "C"

