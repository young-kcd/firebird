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
//	$Id: alice.cpp,v 1.2 2001-07-10 17:35:12 awharrison Exp $
//
// 2001.07.06 Sean Leyne - Code Cleanup, removed "#ifdef READONLY_DATABASE"
//                         conditionals, as the engine now fully supports
//                         readonly databases.
//

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define FB_FROM_ALICE_CPP

#include "../jrd/ib_stdio.h"
#include "../include/jrd/gds.h"
#include "../jrd/common.h"
#include "../jrd/license.h"
#include "../jrd/ibsetjmp.h"
#include "../jrd/msg_encode.h"
#include "../alice/alice.h"
#include "../alice/aliceswi.h"
#include "../alice/all.h"
#include "../alice/alice_proto.h"
#include "../alice/all_proto.h"
#include "../alice/exe_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/svc.h"
#include "../jrd/svc_proto.h"
#include "../jrd/thd_proto.h"

#ifdef SUPERSERVER
#include "../utilities/cmd_util_proto.h"
#endif

#ifdef WIN_NT
#include <windows.h>
#endif

#if (defined WIN_NT || defined PC_PLATFORM)
#include <io.h>
#endif


extern "C" {


#ifndef MAXPATHLEN
#define MAXPATHLEN	1024
#endif

static USHORT val_err_table[] = {
	0,
	55,							/* msg 55: \n\tNumber of record level errors\t: %ld */
	56,							/* msg 56: \tNumber of Blob page errors\t: %ld */
	57,							/* msg 57: \tNumber of data page errors\t: %ld */
	58,							/* msg 58: \tNumber of index page errors\t: %ld */
	59,							/* msg 59: \tNumber of pointer page errors\t: %ld */
	60,							/* msg 60: \tNumber of transaction page errors\t: %ld */
	61							/* msg 61: \tNumber of database page errors\t: %ld */
};

#ifndef NETWARE_386
struct tgbl *gdgbl;
#endif

#define	ALICE_MSG_FAC	3

#if defined (WIN95) && !defined (GUI_TOOLS)
static BOOL fAnsiCP = FALSE;
#define	TRANSLATE_CP(a) if (!fAnsiCP) AnsiToOem(a, a)
#else
#define TRANSLATE_CP(a)
#endif

static void expand_filename(TEXT *, TEXT *);
static int output_thread(SLONG, UCHAR *);
static int output_main(SLONG, UCHAR *);
static int output_svc(SLONG, UCHAR *);
static void alice_output(CONST SCHAR *, ...);



#ifdef SUPERSERVER

//____________________________________________________________
//
//       Entry point for GFIX in case of service manager.
//

int main_gfix(SVC service)
{
	int exit_code;

	exit_code = ALICE_gfix(service->svc_argc, service->svc_argv,
						   output_thread, (SLONG) service);

	service->svc_handle = 0;
	if (service->svc_service->in_use != NULL)
		*(service->svc_service->in_use) = FALSE;

//  Mark service thread as finished.
//  If service is detached, cleanup memory being used by service.
	SVC_finish(service, SVC_finished);

	return exit_code;
}


//____________________________________________________________
//
//		Routine which is passed to GFIX for calling back when there is output.
//

static int output_thread(SLONG output_data, UCHAR * output_buf)
{
	SVC_fprintf((SVC) output_data, "%s", output_buf);

	return 0;
}


#else
#ifndef GUI_TOOLS

//____________________________________________________________
//
//      Call the 'real' main.
//

int CLIB_ROUTINE main(int argc, char *argv[])
{
	int exit_code;

	exit_code = ALICE_gfix(argc, argv, output_main, (SLONG) NULL);

	return exit_code;
}


//____________________________________________________________
//
//		Routine which is passed to GFIX for calling back when there is output.
//

static int output_main(SLONG output_data, UCHAR * output_buf)
{
	ib_fprintf(ib_stderr, "%s", output_buf);
	return 0;
}
#endif
#endif

//____________________________________________________________
//
//		Routine which is passed to GFIX for calling back when there is output
//		if gfix is run as a service
//

static int output_svc(SLONG output_data, UCHAR * output_buf)
{
	ib_fprintf(ib_stdout, "%s", output_buf);
	return 0;
}


//____________________________________________________________
//
//		Routine called by command line utility, and server manager
//		Parse switches and do work
//

int DLL_EXPORT ALICE_gfix(
						  int argc,
						  char *argv[],
						  OUTPUTPROC output_proc, SLONG output_data)
{
	USHORT error, i;
	IN_SW_TAB table = alice_in_sw_table;
	TEXT *database, string[512], *p, *q;
	ULONG switches;
	SLONG redir_in, redir_out, redir_err;
	JMP_BUF env;
	USHORT ret;
#if defined (WIN95) && !defined (GUI_TOOLS)
	BOOL fAnsiCP fAnsiCP = (GetConsoleCP() == GetACP());
#endif

	VOLATILE tgbl *tdgbl = (struct tgbl *) gds__alloc(sizeof(*tdgbl));
//  NOMEM: return error, FREE: during function exit in the SETJMP
	if (tdgbl == NULL)
		return FINI_ERROR;
// TMN
#if 0
	SET_THREAD_DATA;
#else
	tdgbl->tgbl_thd_data.thdd_type = THDD_TYPE_TALICE;
#endif
	SVC_PUTSPECIFIC_DATA;
	memset((void *) tdgbl, 0, sizeof(*tdgbl));
//  TMN: I can't for my life understand why the jmp_buf is defined
//  as a UCHAR* in ' struct tgbl', but it forces this cast.
	tdgbl->alice_env = (UCHAR * volatile) env;
	tdgbl->output_proc = output_proc;
	tdgbl->output_data = output_data;
	tdgbl->ALICE_permanent_pool = NULL;
	tdgbl->ALICE_default_pool = NULL;
	tdgbl->pools = NULL;

	if (SETJMP(env)) {
		int exit_code;

		/* All calls to EXIT(), normal and error exits, wind up here */

		SVC_STARTED(tdgbl->service_blk);
		tdgbl->alice_env = NULL;
		exit_code = tdgbl->exit_code;

		/* Close the status output file */
		if (tdgbl->sw_redirect == TRUE && tdgbl->output_file != NULL) {
			ib_fclose(tdgbl->output_file);
			tdgbl->output_file = NULL;
		}

		/* Free all unfreed memory used by Gfix itself */
		ALLA_fini();

		RESTORE_THREAD_DATA;
		if (tdgbl != NULL) {
			gds__free((SLONG *) tdgbl);
		}
#if defined(DEBUG_GDS_ALLOC) && !defined(SUPERSERVER)
		gds_alloc_report(0, __FILE__, __LINE__);
#endif

		/* All returns occur from this point - even normal returns */
		return exit_code;
	}


#ifdef VMS
	argc = VMS_parse(&argv, argc);
#endif

//  Perform some special handling when run as an Interbase service.  The
//  first switch can be "-svc" (lower case!) or it can be "-svc_re" followed
//  by 3 file descriptors to use in re-directing ib_stdin, ib_stdout, and ib_stderr.

	tdgbl->sw_service = FALSE;
	tdgbl->sw_service_thd = FALSE;
	tdgbl->service_blk = NULL;
	tdgbl->status =
		/* TMN: cast away volatile */
		(long *) tdgbl->status_vector;

	if (argc > 1 && !strcmp(argv[1], "-svc")) {
		tdgbl->sw_service = TRUE;
		argv++;
		argc--;
	}
	else if (argc > 1 && !strcmp(argv[1], "-svc_thd")) {
		tdgbl->sw_service = TRUE;
		tdgbl->sw_service_thd = TRUE;
		tdgbl->service_blk = (SVC) output_data;
		tdgbl->status = tdgbl->service_blk->svc_status;
		argv++;
		argc--;
	}
#ifndef NETWARE_386
	else if (argc > 4 && !strcmp(argv[1], "-svc_re")) {
		tdgbl->sw_service = TRUE;
		tdgbl->output_proc = output_svc;
		redir_in = atol(argv[2]);
		redir_out = atol(argv[3]);
		redir_err = atol(argv[4]);
#ifdef WIN_NT
#if defined (WIN95) && !defined (GUI_TOOLS)
		fAnsiCP = TRUE;
#endif
		redir_in = _open_osfhandle(redir_in, 0);
		redir_out = _open_osfhandle(redir_out, 0);
		redir_err = _open_osfhandle(redir_err, 0);
#endif
		if (redir_in != 0)
			if (dup2((int) redir_in, 0))
				close((int) redir_in);
		if (redir_out != 1)
			if (dup2((int) redir_out, 1))
				close((int) redir_out);
		if (redir_err != 2)
			if (dup2((int) redir_err, 2))
				close((int) redir_err);
		argv += 4;
		argc -= 4;
	}
#endif

	tdgbl->ALICE_data.ua_user = NULL;
	tdgbl->ALICE_data.ua_password = NULL;

//  Start by parsing switches

	error = 0;
	switches = 0;
	tdgbl->ALICE_data.ua_shutdown_delay = 0;
	database = NULL;
	argv++;

	while (--argc > 0) {
		if ((*argv)[0] != '-') {
			if (database)
				ALICE_error(1, database, 0, 0, 0, 0);	/* msg 1: "data base file name (%s) already given", */
			database = *argv++;

#if defined (WIN95) && !defined (GUI_TOOLS)
//  There is a small problem with SuperServer on NT, since it is a
//  Windows app, it uses the ANSI character set.  All the console
//  apps use the OEN character set.  We need to pass the database
//  name in the correct character set.
// if (GetConsoleCP != GetACP())
//    OemToAnsi(database, database);
//
#else
#endif
			continue;
		}
		ALICE_down_case(*argv++, string);
		if (!string[1])
			continue;
		for (table = alice_in_sw_table; TRUE; table++) {
			if (!(p = (TEXT *) table->in_sw_name)) {
				ALICE_print(2, *--argv, 0, 0, 0, 0);	/* msg 2: invalid switch %s */
				error = TRUE;
				break;
			}
			q = &string[1];
			while (*q && *p++ == *q)
				q++;
			if (!*q)
				break;
		}
		if (error)
			break;
		if (*table->in_sw_name == 'x')
			tdgbl->ALICE_data.ua_debug++;
		if (table->in_sw_value == sw_z)
			ALICE_print(3, GDS_VERSION, 0, 0, 0, 0);	/* msg 3: gfix version %s */
		if ((table->in_sw_incompatibilities & switches) ||
			(table->in_sw_requires && !(table->in_sw_requires & switches))) {
			ALICE_print(4, 0, 0, 0, 0, 0);	/* msg 4: incompatible switch combination */
			error = TRUE;
			break;
		}
		switches |= table->in_sw_value;

		if (table->in_sw_value & sw_begin_log) {
			if (--argc <= 0)
				ALICE_error(5, 0, 0, 0, 0, 0);	/* msg 5: replay log pathname required */
			expand_filename(*argv++,	/* TMN: cast away volatile */
							(TEXT *) tdgbl->ALICE_data.ua_log_file);
		}

		if (table->in_sw_value & (sw_buffers)) {
			if (--argc <= 0)
				ALICE_error(6, 0, 0, 0, 0, 0);	/* msg 6: number of page buffers for cache required */
			ALICE_down_case(*argv++, string);
			if ((!(tdgbl->ALICE_data.ua_page_buffers = atoi(string)))
				&& (strcmp(string, "0")))
				ALICE_error(7, 0, 0, 0, 0, 0);	/* msg 7: numeric value required */
			if (tdgbl->ALICE_data.ua_page_buffers < 0)
				ALICE_error(8, 0, 0, 0, 0, 0);	/* msg 8: positive numeric value required */
		}

		if (table->in_sw_value & (sw_housekeeping)) {
			if (--argc <= 0)
				ALICE_error(9, 0, 0, 0, 0, 0);	/* msg 9: number of transactions per sweep required */
			ALICE_down_case(*argv++, string);
			if ((!(tdgbl->ALICE_data.ua_sweep_interval = atoi(string)))
				&& (strcmp(string, "0")))
				ALICE_error(7, 0, 0, 0, 0, 0);	/* msg 7: numeric value required */
			if (tdgbl->ALICE_data.ua_sweep_interval < 0)
				ALICE_error(8, 0, 0, 0, 0, 0);	/* msg 8: positive numeric value required */
		}

		if (table->in_sw_value & (sw_set_db_dialect)) {
			if (--argc <= 0)
				ALICE_error(9, 0, 0, 0, 0, 0);	/* msg 9: dialect info is required XXX */

			ALICE_down_case(*argv++, string);

			if ((!(tdgbl->ALICE_data.ua_db_SQL_dialect = atoi(string))) &&
				(strcmp(string, "0")))
				ALICE_error(7, 0, 0, 0, 0, 0);	/* msg 7: numeric value required */

			if (tdgbl->ALICE_data.ua_db_SQL_dialect < 0)
				ALICE_error(8, 0, 0, 0, 0, 0);	/* msg 8: positive numeric value
												   required */
		}

		if (table->in_sw_value & (sw_commit | sw_rollback | sw_two_phase)) {
			if (--argc <= 0)
				ALICE_error(10, 0, 0, 0, 0, 0);	/* msg 10: transaction number or "all" required */
			ALICE_down_case(*argv++, string);
			if (!(tdgbl->ALICE_data.ua_transaction = atoi(string)))
				if (strcmp(string, "all"))
					ALICE_error(10, 0, 0, 0, 0, 0);	/* msg 10: transaction number or "all" required */
				else
					switches |= sw_list;
		}

		if (table->in_sw_value & sw_write) {
			if (--argc <= 0)
				ALICE_error(11, 0, 0, 0, 0, 0);	/* msg 11: "sync" or "async" required */
			ALICE_down_case(*argv++, string);
			if (!strcmp(string, ALICE_SW_SYNC))
				tdgbl->ALICE_data.ua_force = TRUE;
			else if (!strcmp(string, ALICE_SW_ASYNC))
				tdgbl->ALICE_data.ua_force = FALSE;
			else
				ALICE_error(11, 0, 0, 0, 0, 0);	/* msg 11: "sync" or "async" required */
		}

		if (table->in_sw_value & sw_use) {
			if (--argc <= 0)
				ALICE_error(12, 0, 0, 0, 0, 0);	/* msg 12: "full" or "reserve" required */
			ALICE_down_case(*argv++, string);
			if (!strcmp(string, "full"))
				tdgbl->ALICE_data.ua_use = TRUE;
			else if (!strcmp(string, "reserve"))
				tdgbl->ALICE_data.ua_use = FALSE;
			else
				ALICE_error(12, 0, 0, 0, 0, 0);	/* msg 12: "full" or "reserve" required */
		}

		if (table->in_sw_value & sw_user) {
			if (--argc <= 0)
				ALICE_error(13, 0, 0, 0, 0, 0);	/* msg 13: user name required */
			tdgbl->ALICE_data.ua_user =
				const_cast < UCHAR * volatile >(reinterpret_cast <
												UCHAR * >(*argv++));
		}

		if (table->in_sw_value & sw_password) {
			if (--argc <= 0)
				ALICE_error(14, 0, 0, 0, 0, 0);	/* msg 14: password required */
			tdgbl->ALICE_data.ua_password =
				const_cast < UCHAR * volatile >(reinterpret_cast <
												UCHAR * >(*argv++));
		}

		if (table->in_sw_value & sw_disable) {
			if (--argc <= 0)
				ALICE_error(15, 0, 0, 0, 0, 0);	/* msg 15: subsystem name  */
			ALICE_down_case(*argv++, string);
			if (strcmp(string, "wal"))
				ALICE_error(16, 0, 0, 0, 0, 0);	/* msg 16: "wal" required */
		}

		if (table->in_sw_value & (sw_attach | sw_force | sw_tran | sw_cache)) {
			if (--argc <= 0)
				ALICE_error(17, 0, 0, 0, 0, 0);	/* msg 17: number of seconds required */
			ALICE_down_case(*argv++, string);
			if ((!(tdgbl->ALICE_data.ua_shutdown_delay = atoi(string)))
				&& (strcmp(string, "0")))
				ALICE_error(7, 0, 0, 0, 0, 0);	/* msg 7: numeric value required */
			if (tdgbl->ALICE_data.ua_shutdown_delay < 0
				|| tdgbl->ALICE_data.ua_shutdown_delay > 32767)
				ALICE_error(18, 0, 0, 0, 0, 0);	/* msg 18: numeric value between 0 and 32767 inclusive required */
		}

		if (table->in_sw_value & sw_mode) {
			if (--argc <= 0)
				ALICE_error(110, 0, 0, 0, 0, 0);	/* msg 110: "read_only" or "read_write" required */
			ALICE_down_case(*argv++, string);
			if (!strcmp(string, ALICE_SW_MODE_RO))
				tdgbl->ALICE_data.ua_read_only = TRUE;
			else if (!strcmp(string, ALICE_SW_MODE_RW))
				tdgbl->ALICE_data.ua_read_only = FALSE;
			else
				ALICE_error(110, 0, 0, 0, 0, 0);	/* msg 110: "read_only" or "read_write" required */
		}

	}

//  put this here since to put it above overly complicates the parsing
//  can't use tbl_requires since it only looks backwards on command line
	if ((switches & sw_shut)
		&& !(switches & ((sw_attach | sw_force | sw_tran | sw_cache))))
		ALICE_error(19, 0, 0, 0, 0, 0);	/* msg 19: must specify type of shutdown */

//  catch the case where -z is only command line option
//  switches is unset since sw_z == 0
	if (!switches && !error && table->in_sw_value == sw_z)
		EXIT(FINI_OK);

	if (!switches || !(switches & ~(sw_user | sw_password))) {
#ifndef SUPERSERVER
		ALICE_print(20, 0, 0, 0, 0, 0);	/* msg 20: please retry, specifying an option */
#endif
		error = TRUE;
	}

	if (error) {
#ifdef SUPERSERVER
		CMD_UTIL_put_svc_status(tdgbl->service_blk->svc_status, ALICE_MSG_FAC,
								20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

		SVC_STARTED(tdgbl->service_blk);
#else
		ALICE_print(21, 0, 0, 0, 0, 0);	/* msg 21: plausible options are:\n */
		for (table = alice_in_sw_table; table->in_sw_msg; table++)
			ALICE_print(table->in_sw_msg, 0, 0, 0, 0, 0);
		ALICE_print(22, 0, 0, 0, 0, 0);	/* msg 22: \n    qualifiers show the major option in parenthesis */
#endif
		EXIT(FINI_ERROR);
	}

	if (!database)
		ALICE_error(23, 0, 0, 0, 0, 0);	/* msg 23: please retry, giving a database name */

//  generate the database parameter block for the attach,
//  based on the various switches

	if (switches & (sw_list | sw_commit | sw_rollback | sw_two_phase))
		ret = EXE_two_phase(database, switches);
	else {
		ret = EXE_action(database, switches);

		USHORT count = 0;

		for (i = 0; i < MAX_VAL_ERRORS; i++)
			if (tdgbl->ALICE_data.ua_val_errors[i])
				count++;

		if ((count)
			&& !(tdgbl->ALICE_data.ua_val_errors[VAL_INVALID_DB_VERSION])) {
			ALICE_print(24, 0, 0, 0, 0, 0);	/* msg 24: Summary of validation errors\n */

			for (i = 0; i < MAX_VAL_ERRORS; i++)
				if (tdgbl->ALICE_data.ua_val_errors[i])
					ALICE_print(val_err_table[i],
								reinterpret_cast <
								char *>(tdgbl->ALICE_data.ua_val_errors[i]),
								0, 0, 0, 0);
		}
	}

	if (ret == FINI_ERROR)
		ALICE_print_status(tdgbl->status);

	EXIT(FINI_OK);

	return 0;					// compiler silencer
}


//____________________________________________________________
//
//		Copy a string, down casing as we go.
//

void ALICE_down_case(TEXT * in, TEXT * out)
{
	TEXT c;

	while (c = *in++)
		*out++ = (c >= 'A' && c <= 'Z') ? c - 'A' + 'a' : c;

	*out = 0;
}


//____________________________________________________________
//
//		Display a formatted error message
//

void ALICE_print(USHORT number,
				 TEXT * arg1,
				 TEXT * arg2, TEXT * arg3, TEXT * arg4, TEXT * arg5)
{
	TEXT buffer[256];

	gds__msg_format(0, ALICE_MSG_FAC, number, sizeof(buffer), buffer, arg1,
					arg2, arg3, arg4, arg5);
	TRANSLATE_CP(buffer);
	alice_output("%s\n", buffer);
}


//____________________________________________________________
//
//		Print error message. Use isc_interprete
//		to allow redirecting output.
//

void ALICE_print_status(STATUS * status_vector)
{
#ifdef SUPERSERVER
	TGBL tdgbl;
	STATUS *status;
	int i = 0, j;
#endif
	STATUS *vector;
	SCHAR s[1024];

	if (status_vector) {
		vector = status_vector;
#ifdef SUPERSERVER
		tdgbl = GET_THREAD_DATA;
		status = tdgbl->service_blk->svc_status;
		if (status != status_vector) {
			while (*status && (++i < ISC_STATUS_LENGTH))
				status++;
			for (j = 0; status_vector[j] && (i < ISC_STATUS_LENGTH); j++, i++)
				*status++ = status_vector[j];
		}
#endif
		isc_interprete(s, &vector);
		TRANSLATE_CP(s);
		alice_output("%s\n", s);

		/* Continuation of error */
		s[0] = '-';
		while (isc_interprete(s + 1, &vector)) {
			TRANSLATE_CP(s);
			alice_output("%s\n", s);
		}
	}
}


//____________________________________________________________
//
//		Format and print an error message, then punt.
//

void ALICE_error(USHORT number,
				 TEXT * arg1,
				 TEXT * arg2, TEXT * arg3, TEXT * arg4, TEXT * arg5)
{
	TGBL tdgbl;
	TEXT buffer[256];
#ifdef SUPERSERVER
	STATUS *status;
#endif

	tdgbl = GET_THREAD_DATA;

#ifdef SUPERSERVER
	status = tdgbl->service_blk->svc_status;

	CMD_UTIL_put_svc_status(status, ALICE_MSG_FAC, number,
							isc_arg_string, arg1,
							isc_arg_string, arg2,
							isc_arg_string, arg3,
							isc_arg_string, arg4, isc_arg_string, arg5);
#endif

	gds__msg_format(0, ALICE_MSG_FAC, number, sizeof(buffer), buffer, arg1,
					arg2, arg3, arg4, arg5);
	TRANSLATE_CP(buffer);
	alice_output("%s\n", buffer);
	EXIT(FINI_ERROR);
}


//____________________________________________________________
//
//		Platform independent output routine.
//

static void alice_output(CONST SCHAR * format, ...)
{
	va_list arglist;
	UCHAR buf[1000];
	int exit_code;
	TGBL tdgbl;

	tdgbl = GET_THREAD_DATA;

	if (tdgbl->sw_redirect == NOOUTPUT || format[0] == '\0') {
		exit_code =
			tdgbl->output_proc(tdgbl->output_data,
							   reinterpret_cast < UCHAR * >(""));
	}
	else if (tdgbl->sw_redirect == TRUE && tdgbl->output_file != NULL) {
		VA_START(arglist, format);
		ib_vfprintf(tdgbl->output_file, format, arglist);
		va_end(arglist);
		exit_code =
			tdgbl->output_proc(tdgbl->output_data,
							   reinterpret_cast < UCHAR * >(""));
	}
	else {
		VA_START(arglist, format);
		vsprintf((char *) buf, format, arglist);
		va_end(arglist);

		exit_code = tdgbl->output_proc(tdgbl->output_data, buf);
	}

	if (exit_code != 0)
		EXIT(exit_code);
}


//____________________________________________________________
//
//		Fully expand a file name.  If the file doesn't exist, do something
//		intelligent.
//

static void expand_filename(TEXT * filename, TEXT * expanded_name)
{
	strcpy(expanded_name, filename);
}


} // extern "C"