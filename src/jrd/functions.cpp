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
#include "../jrd/tra.h"
#include "../jrd/dsc_proto.h"

using namespace Jrd;

struct FN {
	const char* fn_module;
	const char* fn_entrypoint;
	FPTR_INT fn_function;
};


static int test(const long*, char*);
static DSC* ni(DSC*, DSC*);
static SLONG* byteLen(const dsc*);
static SLONG set_context(const vary* ns_vary, const vary* name_vary, const vary* value_vary);
static vary* get_context(const vary* ns_vary, const vary* name_vary);

#define FUNCTION(ROUTINE, FUNCTION_NAME, MODULE_NAME, ENTRYPOINT, RET_ARG) \
	{MODULE_NAME, ENTRYPOINT, (FPTR_INT) ROUTINE},
#define END_FUNCTION
#define FUNCTION_ARGUMENT(MECHANISM, TYPE, SCALE, LENGTH, SUB_TYPE, CHARSET, PRECISION, CHAR_LENGTH)

static const FN isc_functions[] = {
	{"test_module", "test_function", (FPTR_INT) test},
	{"test_module", "ni", (FPTR_INT) ni},
	{"test_module", "ns", (FPTR_INT) ni},
	{"test_module", "nn", (FPTR_INT) ni},
	{"test_module", "byte_len", (FPTR_INT) byteLen},

#include "../jrd/functions.h"

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

vary* get_context(const vary* ns_vary, const vary* name_vary)
{
	// Complain if namespace or variable name is null
	if (!ns_vary || !name_vary) {
		ERR_post(isc_ctx_bad_argument, isc_arg_string, "RDB$SET_CONTEXT", 0);
	}

	thread_db* tdbb = JRD_get_thread_data();

	if (!tdbb) {
		fb_assert(false);
		return NULL;
	}

	Firebird::string ns_str(ns_vary->vary_string, ns_vary->vary_length),
		name_str(name_vary->vary_string, name_vary->vary_length);

	// Handle system variables
	if (ns_str == "SYS_SESSION") 
	{
		if (name_str == "DB_NAME") 
		{
			Database* dbb = tdbb->tdbb_database;

			if (!dbb) {
				fb_assert(false);
				return NULL;
			}

			vary *result_vary = (vary*) malloc(dbb->dbb_database_name.length() + 2);
			result_vary->vary_length = dbb->dbb_database_name.length();
			memcpy(result_vary->vary_string, dbb->dbb_database_name.c_str(), result_vary->vary_length);
			return result_vary;
		}
		// "Context variable %s is not found in namespace %s"
		ERR_post(isc_ctx_var_not_found, 
			isc_arg_string, ERR_cstring(name_str.c_str()),
			isc_arg_string,	ERR_cstring(ns_str.c_str()), 0);
	}

	if (ns_str == "SYS_TRANSACTION") 
	{
		if (name_str == "ISOLATION") 
		{
			jrd_tra *transaction = tdbb->tdbb_transaction;

			if (!transaction) {
				fb_assert(false);
				return NULL;
			}

			const char* isolation;

			if (transaction->tra_flags & TRA_read_committed)
				isolation = "READ COMMITTED";
			else if (transaction->tra_flags & TRA_degree3)
				isolation = "CONSISTENCY";
			else
				isolation = "SNAPSHOT";

			vary *result_vary = (vary*) malloc(strlen(isolation) + 2);
			result_vary->vary_length = strlen(isolation);
			memcpy(result_vary->vary_string, isolation, result_vary->vary_length);
			return result_vary;
		}
		// "Context variable %s is not found in namespace %s"
		ERR_post(isc_ctx_var_not_found,
			isc_arg_string, ERR_cstring(name_str.c_str()),
			isc_arg_string, ERR_cstring(ns_str.c_str()), 0);
	}

	// Handle user-defined variables
	Firebird::string result_str;

	if (ns_str == "USER_SESSION") 
	{
		Attachment* att = tdbb->tdbb_attachment;

		if (!att) {
			fb_assert(false);
			return NULL;
		}

		if (!att->att_context_vars.get(name_str, result_str))
			return NULL;
	} else if (ns_str == "USER_TRANSACTION") {
		jrd_tra* tra = tdbb->tdbb_transaction;

		if (!tra) {
			fb_assert(false);
			return NULL;
		}

		if (!tra->tra_context_vars.get(name_str, result_str))
			return NULL;
	} 
	else
	{
		// "Invalid namespace name %s passed to %s"
		ERR_post(isc_ctx_namespace_invalid,
			isc_arg_string, ERR_cstring(ns_str.c_str()),
			isc_arg_string, "RDB$GET_CONTEXT", 0);
	}

	vary *result_vary = (vary*) malloc(result_str.length() + 2);
	result_vary->vary_length = result_str.length();
	memcpy(result_vary->vary_string, result_str.c_str(), result_vary->vary_length);
	return result_vary;
}

static SLONG set_context(const vary* ns_vary, const vary* name_vary, const vary* value_vary)
{
	// Complain if namespace or variable name is null
	if (!ns_vary || !name_vary)
	{
		ERR_post(isc_ctx_bad_argument, isc_arg_string, "RDB$SET_CONTEXT", 0);
	}

	thread_db* tdbb = JRD_get_thread_data();

	if (!tdbb) {
		// Something is seriously wrong
		fb_assert(false);
		return 0;
	}

	Firebird::string ns_str(ns_vary->vary_string, ns_vary->vary_length),
		name_str(name_vary->vary_string, name_vary->vary_length);

	if (ns_str == "USER_SESSION") 
	{
		Attachment* att = tdbb->tdbb_attachment;

		if (!att) {
			fb_assert(false);
			return 0;
		}

		if (!value_vary)
			return att->att_context_vars.remove(name_str);

		if (att->att_context_vars.count() > MAX_CONTEXT_VARS) {
			// "Too many context variables"
			ERR_post(isc_ctx_too_big, 0);
		}

		return att->att_context_vars.put(name_str,
			Firebird::string(value_vary->vary_string, value_vary->vary_length));
	} else if (ns_str == "USER_TRANSACTION") {
		jrd_tra* tra = tdbb->tdbb_transaction;

		if (!tra) {
			fb_assert(false);
			return 0;
		}

		if (!value_vary)
			return tra->tra_context_vars.remove(name_str);

		if (tra->tra_context_vars.count() > MAX_CONTEXT_VARS) {
			// "Too many context variables"
			ERR_post(isc_ctx_too_big, 0);
		}

		return tra->tra_context_vars.put(name_str,
			Firebird::string(value_vary->vary_string, value_vary->vary_length));
	} else {
		// "Invalid namespace name %s passed to %s"
		ERR_post(isc_ctx_namespace_invalid,
			isc_arg_string, ERR_cstring(ns_str.c_str()),
			isc_arg_string, "RDB$SET_CONTEXT", 0);
		return 0;
	}
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
