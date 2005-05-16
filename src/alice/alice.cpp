//____________________________________________________________
//
//		PROGRAM:	Alice (All Else) Utility
//		MODULE:		alice.cpp
//		DESCRIPTION:	Neo-Debe (does everything but eat)
//
//  The contents of this file are subject to the Interbase Public
//  License Version 1.0 (the "License"); you may not use this file
//  except in compliance with the License. You may obtain a copy
//  of the License at http://www.Inprise.com/IPL.html
//
//  Software distributed under the License is distributed on an
//  "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express
//  or implied. See the License for the specific language governing
//  rights and limitations under the License.
//
//  The Original Code was created by Inprise Corporation
//  and its predecessors. Portions created by Inprise Corporation are
//  Copyright (C) Inprise Corporation.
//
//  All Rights Reserved.
//  Contributor(s): ______________________________________.
//
//
//____________________________________________________________
//
// 2001.07.06 Sean Leyne - Code Cleanup, removed "#ifdef READONLY_DATABASE"
//                         conditionals, as the engine now fully supports
//                         readonly databases.
//
// 2002.10.29 Sean Leyne - Removed support for obsolete "Netware" port
//
// 2002.10.30 Sean Leyne - Removed support for obsolete "PC_PLATFORM" define
//
//

#include "firebird.h"
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#include "../jrd/ibase.h"
#include "../jrd/common.h"
#include "../jrd/license.h"
#include "../alice/alice.h"
#include "../alice/aliceswi.h"
#include "../alice/all.h"
#include "../alice/all_proto.h"
#include "../alice/exe_proto.h"
#include "../jrd/msg_encode.h"
#include "../jrd/gds_proto.h"
#include "../jrd/svc.h"
#include "../jrd/svc_proto.h"
#include "../jrd/thd.h"
#include "../alice/alice_proto.h"

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef WIN_NT
#include <io.h>
#endif

#ifdef SUPERSERVER
#include "../utilities/common/cmd_util_proto.h"
#endif

static const USHORT val_err_table[] = {
	0,
	55,				// msg 55: \n\tNumber of record level errors\t: %ld
	56,				// msg 56: \tNumber of Blob page errors\t: %ld
	57,				// msg 57: \tNumber of data page errors\t: %ld
	58,				// msg 58: \tNumber of index page errors\t: %ld
	59,				// msg 59: \tNumber of pointer page errors\t: %ld
	60,				// msg 60: \tNumber of transaction page errors\t: %ld
	61				// msg 61: \tNumber of database page errors\t: %ld
};


// The following structure in only needed if we are building a local exe
// I've commented it out to make it clear since this global variable is
// defined in burp.cpp as well, and is not relevant for SUPERSERVER

#ifndef SUPERSERVER
AliceGlobals* gdgbl;
#endif

const int ALICE_MSG_FAC = 3;

static inline void exit_local(int code, AliceGlobals* tdgbl)
{
	tdgbl->exit_code = code;
	Firebird::status_exception::raise();
}

#if defined (WIN95)
static bool fAnsiCP = false;
#endif

static void ALICE_error(USHORT number);	// overloaded to keep down param count
static inline void translate_cp(TEXT* sz);
static void expand_filename(const TEXT*, TEXT*);
#ifndef SUPERSERVER
static int output_main(Jrd::Service*, const UCHAR*);
#endif
static int common_main(int, char**, Jrd::pfn_svc_output, Jrd::Service*);
static void alice_output(const SCHAR*, ...) ATTRIBUTE_FORMAT(1,2);



//____________________________________________________________
//
//		Routine which is passed to GFIX for calling back when there is output
//		if gfix is run as a service
//

static int output_svc(Jrd::Service* output_data, const UCHAR * output_buf)
{
	fprintf(stdout, "%s", output_buf);
	return 0;
}


#ifdef SUPERSERVER

//____________________________________________________________
//
//	Entry point for GFIX in case of service manager.
//

THREAD_ENTRY_DECLARE ALICE_main(THREAD_ENTRY_PARAM arg)
{
	Jrd::Service* service = (Jrd::Service*)arg;
	const int exit_code = common_main(service->svc_argc, service->svc_argv,
					SVC_output, service);

//  Mark service thread as finished.
//  If service is detached, cleanup memory being used by service.
	SVC_finish(service, Jrd::SVC_finished);

	return (THREAD_ENTRY_RETURN)(exit_code);
}

//____________________________________________________________
//
//	Routine which is passed to GFIX for calling back when there is output.
//

#else	// SUPERSERVER

//____________________________________________________________
//
//      Call the 'real' main.
//

int CLIB_ROUTINE main(int argc, char* argv[])
{
	const int exit_code = common_main(argc, argv, output_main, NULL);

	return exit_code;
}


//____________________________________________________________
//
//		Routine which is passed to GFIX for calling back when there is output.
//

static int output_main(Jrd::Service* output_data, const UCHAR* output_buf)
{
	fprintf(stderr, "%s", output_buf);
	return 0;
}

#endif	// SUPERSERVER

//____________________________________________________________
//
//		Routine called by command line utility, and server manager
//		Parse switches and do work
//

int common_main(int			argc,
				char*		argv[],
				Jrd::pfn_svc_output	output_proc,
				Jrd::Service*		output_data)
{
#if defined (WIN95)
	fAnsiCP = (GetConsoleCP() == GetACP());
#endif

	AliceGlobals gblInstance(output_proc, output_data);
	AliceGlobals* tdgbl = &gblInstance; 
	AliceGlobals::putSpecific(tdgbl);

	try {

#ifdef VMS
	argc = VMS_parse(&argv, argc);
#endif

//  Perform some special handling when run as an Interbase service.  The
//  first switch can be "-svc" (lower case!) or it can be "-svc_re" followed
//  by 3 file descriptors to use in re-directing stdin, stdout, and stderr.

	tdgbl->sw_service = false;
	tdgbl->sw_service_thd = false;
	tdgbl->service_blk = NULL;
	tdgbl->status =	tdgbl->status_vector;

	if (argc > 1 && !strcmp(argv[1], "-svc")) {
		tdgbl->sw_service = true;
		argv++;
		argc--;
	}
	else if (argc > 1 && !strcmp(argv[1], "-svc_thd")) {
		tdgbl->sw_service = true;
		tdgbl->sw_service_thd = true;
		tdgbl->service_blk = (Jrd::Service*) output_data;
		tdgbl->status = tdgbl->service_blk->svc_status;
		argv++;
		argc--;
	}
	else if (argc > 4 && !strcmp(argv[1], "-svc_re")) {
		tdgbl->sw_service = true;
		tdgbl->output_proc = output_svc;
		long redir_in = atol(argv[2]);
		long redir_out = atol(argv[3]);
		long redir_err = atol(argv[4]);
#ifdef WIN_NT
#if defined (WIN95)
		fAnsiCP = true;
#endif
		redir_in = _open_osfhandle(redir_in, 0);
		redir_out = _open_osfhandle(redir_out, 0);
		redir_err = _open_osfhandle(redir_err, 0);
#endif
		if (redir_in != 0) {
			if (dup2((int) redir_in, 0)) {
				close((int) redir_in);
			}
		}
		if (redir_out != 1) {
			if (dup2((int) redir_out, 1)) {
				close((int) redir_out);
			}
		}
		if (redir_err != 2) {
			if (dup2((int) redir_err, 2)) {
				close((int) redir_err);
			}
		}
		argv += 4;
		argc -= 4;
	}

	tdgbl->ALICE_data.ua_user = NULL;
	tdgbl->ALICE_data.ua_password = NULL;

//  Start by parsing switches

	bool error = false;
	ULONG switches = 0;
	tdgbl->ALICE_data.ua_shutdown_delay = 0;
	const TEXT* database = NULL;
	TEXT	string[512];
	
	argv++;
	// tested outside the loop
	const in_sw_tab_t* table = alice_in_sw_table;

	while (--argc > 0)
	{
		if ((*argv)[0] != '-')
		{
			if (database) {
				ALICE_error(1, database, 0, 0, 0, 0);
				// msg 1: "data base file name (%s) already given",
			}
			database = *argv++;

#if defined (WIN95)
//  There is a small problem with SuperServer on NT, since it is a
//  Windows app, it uses the ANSI character set.  All the console
//  apps use the OEM character set.  We need to pass the database
//  name in the correct character set.
// if (GetConsoleCP != GetACP())
//    OemToAnsi(database, database);
//
#else
#endif
			continue;
		}
		ALICE_down_case(*argv++, string, sizeof(string));
		if (!string[1]) {
			continue;
		}
		for (table = alice_in_sw_table; true; ++table)
		{
			const TEXT* p = (TEXT*) table->in_sw_name;
			if (!p) {
				ALICE_print(2, *--argv, 0, 0, 0, 0);	// msg 2: invalid switch %s 
				error = true;
				break;
			}

			TEXT* q = &string[1];
			while (*q && *p++ == *q) {
				q++;
			}
			if (!*q) {
				break;
			}
		}
		if (error) {
			break;
		}
		if (*table->in_sw_name == 'x') {
			tdgbl->ALICE_data.ua_debug++;
		}
		if (table->in_sw_value == sw_z) {
			ALICE_print(3, GDS_VERSION, 0, 0, 0, 0);	// msg 3: gfix version %s 
		}
		if ((table->in_sw_incompatibilities & switches) ||
			(table->in_sw_requires && !(table->in_sw_requires & switches))) {
			ALICE_print(4, 0, 0, 0, 0, 0);	// msg 4: incompatible switch combination 
			error = true;
			break;
		}
		switches |= table->in_sw_value;

		if ((table->in_sw_value & (sw_shut | sw_online)) && (argc > 1)) {
			ALICE_down_case(*argv, string, sizeof(string));
			bool found = false;
			if ((found = (strcmp(string, "normal") == 0)))
				tdgbl->ALICE_data.ua_shutdown_mode = SHUT_NORMAL;
			else
			if ((found = (strcmp(string, "multi") == 0)))
				tdgbl->ALICE_data.ua_shutdown_mode = SHUT_MULTI;
			else
			if ((found = (strcmp(string, "single") == 0)))
				tdgbl->ALICE_data.ua_shutdown_mode = SHUT_SINGLE;
			else
			if ((found = (strcmp(string, "full") == 0)))
				tdgbl->ALICE_data.ua_shutdown_mode = SHUT_FULL;
			// Consume argument only if we identified mode
			// Let's hope that database with names of modes above are unusual
			if (found) {
				argv++;
				argc--;
			}
		}

		if (table->in_sw_value & sw_begin_log) {
			if (--argc <= 0) {
				ALICE_error(5);	// msg 5: replay log pathname required 
			}
			expand_filename(*argv++, tdgbl->ALICE_data.ua_log_file);
		}

		if (table->in_sw_value & (sw_buffers)) {
			if (--argc <= 0) {
				ALICE_error(6);	// msg 6: number of page buffers for cache required 
			}
			ALICE_down_case(*argv++, string, sizeof(string));
			if ((!(tdgbl->ALICE_data.ua_page_buffers = atoi(string)))
				&& (strcmp(string, "0")))
			{
				ALICE_error(7);	// msg 7: numeric value required 
			}
			if (tdgbl->ALICE_data.ua_page_buffers < 0) {
				ALICE_error(8);	// msg 8: positive numeric value required 
			}
		}

		if (table->in_sw_value & (sw_housekeeping)) {
			if (--argc <= 0) {
				ALICE_error(113);	// msg 113: dialect number required 
			}
			ALICE_down_case(*argv++, string, sizeof(string));
			if ((!(tdgbl->ALICE_data.ua_sweep_interval = atoi(string)))
				&& (strcmp(string, "0")))
			{
				ALICE_error(7);	// msg 7: numeric value required 
			}
			if (tdgbl->ALICE_data.ua_sweep_interval < 0) {
				ALICE_error(8);	// msg 8: positive numeric value required 
			}
		}

		if (table->in_sw_value & (sw_set_db_dialect)) {
			if (--argc <= 0) {
				ALICE_error(113);	// msg 113: dialect info is required XXX 
			}

			ALICE_down_case(*argv++, string, sizeof(string));

			if ((!(tdgbl->ALICE_data.ua_db_SQL_dialect = atoi(string))) &&
				(strcmp(string, "0")))
			{
				ALICE_error(7);	// msg 7: numeric value required 
			}

			// JMB: Removed because tdgbl->ALICE_data.ua_db_SQL_dialect is
			//		an unsigned number.  Therefore this check is useless.
			// if (tdgbl->ALICE_data.ua_db_SQL_dialect < 0)
			// {
			//	ALICE_error(8);	/* msg 8: positive numeric value
			//									   required */
			// }
		}

		if (table->in_sw_value & (sw_commit | sw_rollback | sw_two_phase)) {
			if (--argc <= 0) {
				ALICE_error(10);	// msg 10: transaction number or "all" required 
			}
			ALICE_down_case(*argv++, string, sizeof(string));
			if (!(tdgbl->ALICE_data.ua_transaction = atoi(string))) {
				if (strcmp(string, "all")) {
					ALICE_error(10);	// msg 10: transaction number or "all" required 
				}
				else {
					switches |= sw_list;
				}
			}
		}

		if (table->in_sw_value & sw_write) {
			if (--argc <= 0) {
				ALICE_error(11);	// msg 11: "sync" or "async" required 
			}
			ALICE_down_case(*argv++, string, sizeof(string));
			if (!strcmp(string, ALICE_SW_SYNC)) {
				tdgbl->ALICE_data.ua_force = true;
			}
			else if (!strcmp(string, ALICE_SW_ASYNC)) {
				tdgbl->ALICE_data.ua_force = false;
			}
			else {
				ALICE_error(11);	// msg 11: "sync" or "async" required 
			}
		}

		if (table->in_sw_value & sw_use) {
			if (--argc <= 0) {
				ALICE_error(12);	// msg 12: "full" or "reserve" required 
			}
			ALICE_down_case(*argv++, string, sizeof(string));
			if (!strcmp(string, "full")) {
				tdgbl->ALICE_data.ua_use = true;
			}
			else if (!strcmp(string, "reserve")) {
				tdgbl->ALICE_data.ua_use = false;
			}
			else {
				ALICE_error(12);	// msg 12: "full" or "reserve" required 
			}
		}

		if (table->in_sw_value & sw_user) {
			if (--argc <= 0) {
				ALICE_error(13);	// msg 13: user name required 
			}
			tdgbl->ALICE_data.ua_user = reinterpret_cast<UCHAR*>(*argv++);
		}

		if (table->in_sw_value & sw_password) {
			if (--argc <= 0) {
				ALICE_error(14);	// msg 14: password required 
			}
			tdgbl->ALICE_data.ua_password = reinterpret_cast<UCHAR*>(*argv++);
		}

		if (table->in_sw_value & sw_disable) {
			if (--argc <= 0) {
				ALICE_error(15);	// msg 15: subsystem name  
			}
			ALICE_down_case(*argv++, string, sizeof(string));
			if (strcmp(string, "wal")) {
				ALICE_error(16);	// msg 16: "wal" required 
			}
		}

		if (table->in_sw_value & (sw_attach | sw_force | sw_tran | sw_cache)) {
			if (--argc <= 0) {
				ALICE_error(17);	// msg 17: number of seconds required 
			}
			ALICE_down_case(*argv++, string, sizeof(string));
			if ((!(tdgbl->ALICE_data.ua_shutdown_delay = atoi(string)))
				&& (strcmp(string, "0")))
			{
				ALICE_error(7);	// msg 7: numeric value required 
			}
			if (tdgbl->ALICE_data.ua_shutdown_delay < 0
				|| tdgbl->ALICE_data.ua_shutdown_delay > 32767)
			{
				ALICE_error(18);	// msg 18: numeric value between 0 and 32767 inclusive required 
			}
		}

		if (table->in_sw_value & sw_mode) {
			if (--argc <= 0) {
				ALICE_error(110);	// msg 110: "read_only" or "read_write" required 
			}
			ALICE_down_case(*argv++, string, sizeof(string));
			if (!strcmp(string, ALICE_SW_MODE_RO)) {
				tdgbl->ALICE_data.ua_read_only = true;
			}
			else if (!strcmp(string, ALICE_SW_MODE_RW)) {
				tdgbl->ALICE_data.ua_read_only = false;
			}
			else {
				ALICE_error(110);	// msg 110: "read_only" or "read_write" required 
			}
		}

	}

//  put this here since to put it above overly complicates the parsing
//  can't use tbl_requires since it only looks backwards on command line
	if ((switches & sw_shut)
		&& !(switches & ((sw_attach | sw_force | sw_tran | sw_cache))))
	{
		ALICE_error(19);	// msg 19: must specify type of shutdown 
	}

//  catch the case where -z is only command line option
//  switches is unset since sw_z == 0
	if (!switches && !error && table->in_sw_value == sw_z) {
		exit_local(FINI_OK, tdgbl);
	}

	if (!switches || !(switches & ~(sw_user | sw_password))) {
#ifndef SUPERSERVER
		ALICE_print(20, 0, 0, 0, 0, 0);	// msg 20: please retry, specifying an option 
#endif
		error = true;
	}

	if (error) {
#ifdef SUPERSERVER
		CMD_UTIL_put_svc_status(tdgbl->service_blk->svc_status, ALICE_MSG_FAC,
								20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

		tdgbl->service_blk->svc_started();
#else
		ALICE_print(21, 0, 0, 0, 0, 0);	// msg 21: plausible options are:\n 
		for (table = alice_in_sw_table; table->in_sw_msg; table++)
			ALICE_print(table->in_sw_msg, 0, 0, 0, 0, 0);
		ALICE_print(22, 0, 0, 0, 0, 0);	// msg 22: \n    qualifiers show the major option in parenthesis 
#endif
		exit_local(FINI_ERROR, tdgbl);
	}

	if (!database) {
		ALICE_error(23);	// msg 23: please retry, giving a database name 
	}

	//  generate the database parameter block for the attach,
	//  based on the various switches

	USHORT ret;

	if (switches & (sw_list | sw_commit | sw_rollback | sw_two_phase))
	{
		ret = EXE_two_phase(database, switches);
	}
	else
	{
		ret = EXE_action(database, switches);

		const SLONG* ua_val_errors = tdgbl->ALICE_data.ua_val_errors;

		if (!ua_val_errors[VAL_INVALID_DB_VERSION])
		{
			bool any_error = false;

			for (int i = 0; i < MAX_VAL_ERRORS; ++i) {
				if (ua_val_errors[i]) {
					any_error = true;
					break;
				}
			}

			if (any_error) {
				ALICE_print(24, 0, 0, 0, 0, 0);	// msg 24: Summary of validation errors\n 

				for (int i = 0; i < MAX_VAL_ERRORS; ++i) {
					if (ua_val_errors[i]) {
						TEXT* szErr = reinterpret_cast<TEXT*>(ua_val_errors[i]);
						ALICE_print(val_err_table[i], szErr, 0, 0, 0, 0);
					}
				}
			}
		}
	}

	if (ret == FINI_ERROR) {
		ALICE_print_status(tdgbl->status);
	}

	exit_local(FINI_OK, tdgbl);

	}	// try
	catch (const std::exception&)
	{
		// All "calls" to exit_local(), normal and error exits, wind up here 

		tdgbl->service_blk->svc_started();

		int exit_code = tdgbl->exit_code;

		// Close the status output file 
		if (tdgbl->sw_redirect == REDIRECT && tdgbl->output_file != NULL) 
		{
			fclose(tdgbl->output_file);
			tdgbl->output_file = NULL;
		}

		AliceGlobals::restoreSpecific();

#if defined(DEBUG_GDS_ALLOC) && !defined(SUPERSERVER)
		gds_alloc_report(0, __FILE__, __LINE__);
#endif

		// All returns occur from this point - even normal returns 
		return exit_code;
	}	// catch

	return 0;					// compiler silencer
}


//____________________________________________________________
//
//		Copy a string, down casing as we go.
//

void ALICE_down_case(const TEXT* in, TEXT* out, const size_t buf_size)
{
	const TEXT* const end = out + buf_size - 1;
	for (TEXT c = *in++; c && out < end; c = *in++) {
		*out++ = (c >= 'A' && c <= 'Z') ? c - 'A' + 'a' : c;
	}
	*out = 0;
}


//____________________________________________________________
//
//		Display a formatted error message
//

void ALICE_print(USHORT	number,
				 const TEXT*	arg1,
				 const TEXT*	arg2,
				 const TEXT*	arg3,
				 const TEXT*	arg4,
				 const TEXT*	arg5)
{
	TEXT buffer[256];

	gds__msg_format(0, ALICE_MSG_FAC, number, sizeof(buffer), buffer, arg1,
					arg2, arg3, arg4, arg5);
	translate_cp(buffer);
	alice_output("%s\n", buffer);
}


//____________________________________________________________
//
//		Print error message. Use fb_interpret
//		to allow redirecting output.
//

void ALICE_print_status(const ISC_STATUS* status_vector)
{
	if (status_vector)
	{
		const ISC_STATUS* vector = status_vector;
#ifdef SUPERSERVER
		AliceGlobals* tdgbl = AliceGlobals::getSpecific();
		ISC_STATUS* status = tdgbl->service_blk->svc_status;
		if (status != status_vector) {
			int i = 0;
			if (status[1]) {
				while (*status && (++i < ISC_STATUS_LENGTH)) {
					status++;
				}
			}
			for (int j = 0; status_vector[j] && (i < ISC_STATUS_LENGTH); j++, i++) {
				*status++ = status_vector[j];
			}
		}
#endif

		SCHAR s[1024];
		if (fb_interpret(s, sizeof(s), &vector))
		{
			translate_cp(s);
			alice_output("%s\n", s);

			// Continuation of error
			s[0] = '-';
			while (fb_interpret(s + 1, sizeof(s) - 1, &vector)) {
				translate_cp(s);
				alice_output("%s\n", s);
			}
		}
	}
}


//____________________________________________________________
//
//		Format and print an error message, then punt.
//

void ALICE_error(USHORT	number,
				 const TEXT*	arg1,
				 const TEXT*	arg2,
				 const TEXT*	arg3,
				 const TEXT*	arg4,
				 const TEXT*	arg5)
{
	AliceGlobals* tdgbl = AliceGlobals::getSpecific();
	TEXT buffer[256];

#ifdef SUPERSERVER
	ISC_STATUS* status = tdgbl->service_blk->svc_status;

	CMD_UTIL_put_svc_status(status, ALICE_MSG_FAC, number,
							isc_arg_string, arg1,
							isc_arg_string, arg2,
							isc_arg_string, arg3,
							isc_arg_string, arg4,
							isc_arg_string, arg5);
#endif

	gds__msg_format(0, ALICE_MSG_FAC, number, sizeof(buffer), buffer, arg1,
					arg2, arg3, arg4, arg5);
	translate_cp(buffer);
	alice_output("%s\n", buffer);
	exit_local(FINI_ERROR, tdgbl);
}



//
// Overload of ALICE_error to keep down parameter count.
//
static void ALICE_error(USHORT number)
{
	ALICE_error(number, 0, 0, 0, 0, 0);
}


//____________________________________________________________
//
//		Platform independent output routine.
//

static void alice_output(const SCHAR* format, ...)
{
	va_list arglist;
	UCHAR buf[1000];
	int exit_code;

	AliceGlobals* tdgbl = AliceGlobals::getSpecific();

	if (tdgbl->sw_redirect == NOOUTPUT || format[0] == '\0') {
		exit_code = tdgbl->output_proc(tdgbl->output_data, (UCHAR *)(""));
	}
	else if (tdgbl->sw_redirect == REDIRECT && tdgbl->output_file != NULL) {
		va_start(arglist, format);
		vfprintf(tdgbl->output_file, format, arglist);
		va_end(arglist);
		exit_code = tdgbl->output_proc(tdgbl->output_data, (UCHAR *)(""));
	}
	else {
		va_start(arglist, format);
		vsprintf((char *) buf, format, arglist);
		va_end(arglist);

		exit_code = tdgbl->output_proc(tdgbl->output_data, buf);
	}

	if (exit_code != 0) {
		exit_local(exit_code, tdgbl);
	}
}


//
// Translate the given string from Windows ANSI charset to the
// to the current OEM charset iff:
//  1. The macro WIN95 is defined AND
//  2. The macro GUI_TOOLS is NOT defined AND
//  3. The static variable fAnsiCP is false.
//
static inline void translate_cp(TEXT* sz)
{
#if defined (WIN95)
	if (!fAnsiCP) {
		CharToOem(sz, sz);
	}
#endif
}

//____________________________________________________________
//
//		Fully expand a file name.  If the file doesn't exist, do something
//		intelligent.
//      CVC: The above comment is either a joke or a copy/paste.

static void expand_filename(const TEXT* filename, TEXT* expanded_name)
{
	strcpy(expanded_name, filename);
}

