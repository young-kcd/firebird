/*
 *	PROGRAM:	JRD Backup and Restore Program
 *	MODULE:		burp.c
 *	DESCRIPTION:	Command line interpreter for backup/restore
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
 * 2001.07.06 Sean Leyne - Code Cleanup, removed "#ifdef READONLY_DATABASE"
 *                         conditionals, as the engine now fully supports
 *                         readonly databases.
 *
 * 2002.02.15 Sean Leyne - Code Cleanup, removed obsolete "EPSON" defines
 *
 * 2002.10.29 Sean Leyne - Removed obsolete "Netware" port
 *
// 2002.10.30 Sean Leyne - Removed support for obsolete "PC_PLATFORM" define
//
 */

#include "firebird.h"
#include "../jrd/ib_stdio.h"
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "../jrd/common.h"
#include "../jrd/y_ref.h"
#include "../jrd/ibase.h"
#include <stdarg.h>
#include "../jrd/ibsetjmp.h"
#include "../jrd/msg_encode.h"
#include "../jrd/thd_proto.h"
#include "../jrd/ods.h"			// to get MAX_PAGE_SIZE
#include "../burp/burp.h"
#include "../burp/burpswi.h"
#ifdef WIN_NT
#include <windows.h>
#undef TEXT
#include <winnt.h>
#define TEXT char
#endif

#include "../jrd/license.h"

#include "../jrd/jrd_time.h"
#include "../burp/burp_proto.h"
#include "../burp/backu_proto.h"
#include "../burp/mvol_proto.h"
#include "../burp/resto_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/why_proto.h"
#include "../jrd/gdsassert.h"

#ifdef HAVE_CTYPE_H
#include <ctype.h>
#endif

#ifdef SUPERSERVER
#include "../utilities/cmd_util_proto.h"
#endif


#ifdef UNIX
#include <unistd.h>
#endif

#ifdef VMS
#include <descrip.h>
#include <iodef.h>
#include <types.h>
#include <file.h>
#define SWITCH_CHAR	"/"
#endif

#ifndef VMS
#include <fcntl.h>
#endif

#if (defined WIN_NT)
#include <io.h>
#endif

#ifndef O_CREAT
#include <sys/types.h>
#include <sys/file.h>
#endif

// The following structure in only needed if we are building a local exe
// I've commented it out to make it clear since this global variable is
// defined in burp.cpp as well, and is not relevant for SUPERSERVER
// MOD 23-July-2002

#ifndef SUPERSERVER
struct tgbl *gdgbl;
#endif

#ifndef FOPEN_WRITE_TYPE
#define FOPEN_WRITE_TYPE	"w"
#endif

#ifndef FOPEN_READ_TYPE
#define FOPEN_READ_TYPE		"r"
#endif

#ifndef OPEN_MASK
#define OPEN_MASK		0666
#endif

#ifndef SWITCH_CHAR
#define SWITCH_CHAR	"-"
#endif


#define OUTPUT_SUPPRESS	"SUPPRESS"
#define BURP_MSG_FAC    12

static void close_out_transaction(volatile SSHORT, isc_tr_handle *);
//static void enable_signals(void);
//static void excp_handler(void);
static SLONG get_number(SCHAR *);
static ULONG get_size(SCHAR *, FIL);
static SSHORT open_files(SCHAR *, SCHAR **, USHORT, USHORT, USHORT);
#ifdef SUPERSERVER
static int output_netware(SLONG, UCHAR *);
#else
static int output_main(SLONG, UCHAR *);
#endif
static int output_svc(SLONG, UCHAR *);
static void burp_output(const SCHAR *, ...) ATTRIBUTE_FORMAT(1,2);

#ifndef	SUPERSERVER
static int api_gbak(int, char**, USHORT, TEXT*, TEXT*, TEXT *, BOOLEAN, BOOLEAN);
#endif

#define QUIT		0
#define BACKUP		1
#define RESTORE		2
#define FDESC		3

#define	DB		tdgbl->db_handle

#define GBAK_STDIN_DESC		(int)0
#define GBAK_STDOUT_DESC	(int)1

#define KBYTE	1024
#define	MBYTE	KBYTE * KBYTE
#define GBYTE	MBYTE * KBYTE

#if defined (WIN95)
static BOOL fAnsiCP = FALSE;
#define TRANSLATE_CP(a) if (!fAnsiCP) AnsiToOem(a, a)
#else
#define TRANSLATE_CP(a)
#endif


#ifdef SUPERSERVER
int main_gbak(SVC service)
{
/**************************************
 *
 *	m a i n _ g b a k
 *
 **************************************
 *
 * Functional description
 *	Netware entry point for GBAK.
 *
 **************************************/
	int exit_code;

	exit_code = BURP_gbak(service->svc_argc, service->svc_argv,
						  output_netware, (SLONG) service);

	service->svc_handle = 0;
	if (service->svc_service->in_use != NULL)
		*(service->svc_service->in_use) = FALSE;

/* Mark service thread as finished. */
/* If service is detached, cleanup memory being used by service. */
	SVC_finish(service, SVC_finished);

	return exit_code;
}


static int output_netware(SLONG output_data, UCHAR* output_buf)
{
/**************************************
 *
 *	o u t p u t _ n e t w a r e
 *
 **************************************
 *
 * Functional description
 *	Routine which is passed to GBAK for calling back when there is output.
 *
 **************************************/
	SVC_fprintf((SVC) output_data, "%s", output_buf);

	return 0;
}


void BURP_svc_error(USHORT errcode,
					USHORT arg1_t,
					void *arg1,
					USHORT arg2_t,
					void *arg2,
					USHORT arg3_t,
					void *arg3,
					USHORT arg4_t,
					void *arg4,
					USHORT arg5_t,
					void *arg5)
{
/**************************************
 *
 *      B U R P _ s v c _ e r r o r
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	TGBL tdgbl;
	ISC_STATUS *status;

	tdgbl = GET_THREAD_DATA;

	status = tdgbl->service_blk->svc_status;

	CMD_UTIL_put_svc_status(status, BURP_MSG_FAC, errcode,
							arg1_t, arg1, arg2_t, arg2, arg3_t, arg3,
							arg4_t, arg4, arg5_t, arg5);

	SVC_STARTED(tdgbl->service_blk);
	BURP_msg_partial(256, 0, 0, 0, 0, 0);	/* msg 256: gbak: ERROR: */
	BURP_msg_put(errcode,
				reinterpret_cast<TEXT*>(arg1),
				reinterpret_cast<TEXT*>(arg2),
				reinterpret_cast<TEXT*>(arg3),
				reinterpret_cast<TEXT*>(arg4),
				reinterpret_cast<TEXT*>(arg5));
	BURP_abort();
}


#else	// SUPERSERVER

int CLIB_ROUTINE main(int argc, char* argv[])
{
/**************************************
 *
 *	m a i n
 *
 **************************************
 *
 * Functional description
 *	Parse and interpret command line, then "do the right thing."
 *
 **************************************/
	int exit_code;
	USHORT total;
	TEXT **end, **argvp, *string, *p, *q, c;
	IN_SW_TAB in_sw_tab;
	BOOLEAN flag_restore, flag_verbose, err;
	TEXT *sw_user, *sw_password, *sw_service;
	TEXT *d_user, *d_password, *d_service;

/* If a "-service" switch is specified then use Admin APIs */
	argvp = argv;
	end = argvp + argc;
	argvp++;

/* Initialize data */
	total = 0;
	flag_restore = flag_verbose = err = FALSE;
	sw_user = sw_password = sw_service = d_user = d_password = d_service = NULL;

/* Parse the command line for the -USER, -PASSWORD, -SERVICE,
   and -VERBOSE options. Calculate a length for the new command line to be
   passed to a server using services APIs */

	while (argvp < end && !err)
	{
		string = *argvp++;
		if (*string != '-') {
			total += strlen(string) + 1;
			continue;
		}
		if (!string[1])
			string = "-*NONE*";
		for (in_sw_tab = burp_in_sw_table;
			q = in_sw_tab->in_sw_name;
			in_sw_tab++)
		{
			for (p = string + 1; c = *p++;)
				if (UPPER(c) != *q++)
					break;
			if (!c)
				break;
		}
		switch (in_sw_tab->in_sw)
		{
		case IN_SW_BURP_C:		/* create database */
		case IN_SW_BURP_R:		/* replace database */
			total += strlen(string) + 1;
			flag_restore = TRUE;
			break;
		case IN_SW_BURP_USER:	/* default user name */
			if (argvp >= end)
				err = TRUE;
			else {
				sw_user = string;
				d_user = *argvp++;
			}
			break;
		case IN_SW_BURP_PASS:	/* default password */
			if (argvp >= end)
				err = TRUE;
			else {
				sw_password = string;
				d_password = *argvp++;
			}
			break;
		case IN_SW_BURP_SE:	/* service name */
			if (argvp >= end) {
				err = TRUE;
			} else {
				sw_service = string;
				d_service = *argvp++;
			}
			break;
		case IN_SW_BURP_V:		/* verify actions */
			total += strlen(string) + 1;
			flag_verbose = TRUE;
			break;
		default:
			total += strlen(string) + 1;
			break;
		}
	}

	if (sw_service && !err)
	{
		/* Backup/restore operations will be running as a service thread.
		 * To make this more efficiently the isc_spb_options is used.
		 * This allows us to skip a conversion from the gbak command line
		 * switches to service parameter block in here as well as vice versa
		 * conversion within svc.c
		 *
		 * If -USER and -PASSWORD switches are used by the user within
		 * the gbak command line then we have to eliminate them from there. The
		 * password will be encrypted and added along with the user name
		 * within SVC_start function later on. We shall also eliminate
		 * the -SERVER switch because the switch has already been processed.
		 */

		if (sw_user)
			*sw_user = '\0';
		if (sw_password)
			*sw_password = '\0';
		if (sw_service)
			*sw_service = '\0';

		exit_code = api_gbak(argc, argv, total, d_password,
							 d_user, d_service, flag_restore, flag_verbose);
	}
	else
		exit_code = BURP_gbak(argc, argv, output_main, (SLONG) NULL);

	return exit_code;
}


static int output_main( SLONG output_data, UCHAR * output_buf)
{
/**************************************
 *
 *	o u t p u t _ m a i n
 *
 **************************************
 *
 * Functional description
 *	Routine which is passed to GBAK for calling back when there is output.
 *
 **************************************/
	ib_fprintf(ib_stderr, "%s", output_buf);
	return 0;
}

#endif	// SUPERSERVER


static int output_svc( SLONG output_data, UCHAR * output_buf)
{
/**************************************
 *
 *	o u t p u t _ s v c
 *
 **************************************
 *
 * Functional description
 *	Routine which is passed to GBAK for calling back when there is output
 *      if gbak is run as a service
 *
 **************************************/
	ib_fprintf(ib_stdout, "%s", output_buf);
	return 0;
}

extern "C" {

int DLL_EXPORT BURP_gbak(int		argc,
						 char*		argv[],
						 OUTPUTPROC	output_proc,
						 SLONG		output_data)
{
/**************************************
 *
 *	B U R P _ g b a k
 *
 **************************************
 *
 * Functional description
 *	Routine called by command line utility, services API, and server manager.
 *
 **************************************/
	TEXT *file1, **end, *string, *p, *q, c, *device, *redirect;
/* This function runs within thread for services API, so here should not be
   *any* static variables. I did not change an existing definition
   for AIX PowerPC because of the problem (see comments below). So
   whoever will do a port on AIX, must reconsider a static definition */
#ifdef AIX_PPC
	static TEXT *file2;			/* SomeHow, making this volatile does'nt give the
								   desired value in case of AIX PowerPC */
#else
	TEXT *file2;
#endif
	UCHAR *dpb;

	IN_SW_TAB in_sw_tab;
	FIL file, file_list, next_file;
	int temp, result;
	SLONG clock;
	SLONG redir_in, redir_out, redir_err;
	volatile SSHORT action = QUIT;
	USHORT					sw_replace;
	USHORT					sw_tape;
	volatile struct tgbl*	tdgbl;
	JMP_BUF					env;
	IB_FILE*				tmp_outfile;

/* TMN: This variable should probably be removed, but I left it in */
/* in case some platform should redefine the BURP SET_THREAD_DATA. */
/*struct tgbl	thd_context;*/

	tdgbl = (struct tgbl *) gds__alloc(sizeof(*tdgbl));
/* NOMEM: return error, FREE: during function exit in the SETJMP */
	if (tdgbl == NULL)
	{
		SVC service;
		service = (SVC) output_data;
		SVC_STARTED(service);
		return FINI_ERROR;
	}

	SET_THREAD_DATA;
	SVC_PUTSPECIFIC_DATA;
	memset((void *) tdgbl, 0, sizeof(*tdgbl));
	tdgbl->burp_env = reinterpret_cast<UCHAR*>(env);
	tdgbl->file_desc = INVALID_HANDLE_VALUE;
	tdgbl->output_proc = output_proc;
	tdgbl->output_data = output_data;

/* Initialize static data. */
	for (in_sw_tab = burp_in_sw_table; in_sw_tab->in_sw_name; in_sw_tab++) {
		in_sw_tab->in_sw_state = FALSE;
	}

	try {

#ifdef VMS
	argc = VMS_parse(&argv, argc);
#endif

/* Perform some special handling when run as an Interbase service.  The
   first switch can be "-svc" (lower case!) or it can be "-svc_re" followed
   by 3 file descriptors to use in re-directing stdin, stdout, and stderr.

   If this utility is started as a thread in the engine, then the first switch
   will be "-svc_thd".
*/
	tdgbl->gbl_sw_service_gbak = FALSE;
	tdgbl->gbl_sw_service_thd = FALSE;
	tdgbl->service_blk = NULL;
	tdgbl->status = const_cast<long* volatile>(tdgbl->status_vector);

	if (argc > 1 && !strcmp(argv[1], "-svc")) {
		tdgbl->gbl_sw_service_gbak = TRUE;
		argv++;
		argc--;
	}
	else if (argc > 1 && !strcmp(argv[1], "-svc_thd")) {
		tdgbl->gbl_sw_service_gbak = TRUE;
		tdgbl->gbl_sw_service_thd = TRUE;
		tdgbl->service_blk = (SVC) output_data;
		tdgbl->status = tdgbl->service_blk->svc_status;
		argv++;
		argc--;
	}
	else if (argc > 4 && !strcmp(argv[1], "-svc_re")) {
		tdgbl->gbl_sw_service_gbak = TRUE;
		tdgbl->output_proc = output_svc;
		redir_in = atol(argv[2]);
		redir_out = atol(argv[3]);
		redir_err = atol(argv[4]);
#ifdef WIN_NT
#if defined (WIN95)
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

#if defined (WIN95)
	if (!fAnsiCP)
		fAnsiCP = (GetConsoleCP() == GetACP());
#endif


	sw_replace = sw_tape = FALSE;

	tdgbl->gbl_sw_compress = TRUE;
	tdgbl->gbl_sw_convert_ext_tables = FALSE;
	tdgbl->gbl_sw_transportable = TRUE;
	tdgbl->gbl_sw_ignore_limbo = FALSE;
	tdgbl->gbl_sw_blk_factor = 0;
	tdgbl->gbl_sw_no_reserve = FALSE;
	tdgbl->gbl_sw_mode = FALSE;
	tdgbl->gbl_sw_skip_count = 0;
	tdgbl->action = NULL;
	dpb = const_cast<UCHAR*>(tdgbl->dpb_string);
	tdgbl->dpb_length = 0;
	file1 = file2 = NULL;
	file = file_list = NULL;
	tdgbl->io_buffer_size = GBAK_IO_BUFFER_SIZE;
	end = argv + argc;
	++argv;

	while (argv < end) {
		string = *argv;
		temp = strlen(string) - 1;
		if (string[temp] == ',')
			string[temp] = '\0';

		if (*string != '-') {
			if (!file || file->fil_length || !get_size(*argv, file)) {
				/*  Miserable thing must be a filename
				   (dummy in a length for the backup file */

				file = (FIL) BURP_ALLOC_ZERO(FIL_LEN);
				file->fil_name = string;
				file->fil_fd = INVALID_HANDLE_VALUE;
				if (!file_list)
					file->fil_length = MAX_LENGTH;
				else
					file->fil_length = 0;
				file->fil_next = file_list;
				file_list = file;
			}
			argv++;
		}
		else {
			++argv;
			if (!string[1])
				string = "-*NONE*";
			for (in_sw_tab = burp_in_sw_table; q = in_sw_tab->in_sw_name;
				 in_sw_tab++) {
				for (p = string + 1; c = *p++;)
					if (UPPER(c) != *q++)
						break;
				if (!c)
					break;
			}
			in_sw_tab->in_sw_state = TRUE;
			if (!in_sw_tab->in_sw) {
				BURP_print(137, string + 1, 0, 0, 0, 0);	/* msg 137  unknown switch %s */
				BURP_print(95, 0, 0, 0, 0, 0);	/* msg 95  legal switches are */
				for (in_sw_tab = burp_in_sw_table; in_sw_tab->in_sw;
					 in_sw_tab++)
					if (in_sw_tab->in_sw_msg) {
						BURP_msg_put(in_sw_tab->in_sw_msg, (void*)SWITCH_CHAR, 0, 0,
									 0, 0);
					}

				BURP_print(132, 0, 0, 0, 0, 0);	/* msg 132 switches can be abbreviated to one character */
				BURP_error(1, 0, 0, 0, 0, 0);	/* msg 1: found unknown switch */
			}
			else if (in_sw_tab->in_sw == IN_SW_BURP_S) {
				if (argv >= end)
					BURP_error(200, 0, 0, 0, 0, 0);
				/* msg 200: missing parameter for the number of bytes to be skipped */
				tdgbl->gbl_sw_skip_count = get_number(*argv);
				if (!tdgbl->gbl_sw_skip_count)
					BURP_error(201, *argv, 0, 0, 0, 0);
				/* msg 201: expected number of bytes to be skipped, encountered "%s" */
				argv++;
			}
			else if (in_sw_tab->in_sw == IN_SW_BURP_P) {
				if (argv >= end)
					BURP_error(2, 0, 0, 0, 0, 0);	/* msg 2 page size parameter missing */
				tdgbl->gbl_sw_page_size = (USHORT) get_number(*argv);
				if (!tdgbl->gbl_sw_page_size)
					BURP_error(12, *argv, 0, 0, 0, 0);	/* msg 12 expected page size, encountered "%s" */
				argv++;
			}
			else if (in_sw_tab->in_sw == IN_SW_BURP_BU) {
				if (argv >= end)
					BURP_error(258, 0, 0, 0, 0, 0);	/* msg 258 page buffers parameter missing */
				tdgbl->gbl_sw_page_buffers = get_number(*argv);
				if (!tdgbl->gbl_sw_page_buffers)
					BURP_error(259, *argv, 0, 0, 0, 0);	/* msg 259 expected page buffers, encountered "%s" */
				argv++;
			}
			else if (in_sw_tab->in_sw == IN_SW_BURP_MODE) {
				if (argv >= end)
					BURP_error(279, 0, 0, 0, 0, 0);	/* msg 279: "read_only" or "read_write" required */
				string = *argv++;
				if (!strcmp(string, BURP_SW_MODE_RO))
					tdgbl->gbl_sw_mode_val = TRUE;
				else if (!strcmp(string, BURP_SW_MODE_RW))
					tdgbl->gbl_sw_mode_val = FALSE;
				else
					BURP_error(279, 0, 0, 0, 0, 0);	/* msg 279: "read_only" or "read_write" required */
				tdgbl->gbl_sw_mode = TRUE;
			}
			else if (in_sw_tab->in_sw == IN_SW_BURP_PASS) {
				if (argv >= end)
					BURP_error(189, 0, 0, 0, 0, 0);	/* password parameter missing */
				tdgbl->gbl_sw_password = *argv++;
			}
			else if (in_sw_tab->in_sw == IN_SW_BURP_USER) {
				if (argv >= end)
					BURP_error(188, 0, 0, 0, 0, 0);	/* user name parameter missing */
				tdgbl->gbl_sw_user = *argv++;
			}
			else if (in_sw_tab->in_sw == IN_SW_BURP_ROLE) {
				if (argv >= end)
					BURP_error(253, 0, 0, 0, 0, 0);	/* SQL role parameter missing */
				tdgbl->gbl_sw_sql_role = *argv++;
			}
			else if (in_sw_tab->in_sw == IN_SW_BURP_FA) {
				if (argv >= end)
					BURP_error(182, 0, 0, 0, 0, 0);	/* msg 182 blocking factor parameter missing */
				tdgbl->gbl_sw_blk_factor =
					(volatile USHORT) get_number(*argv);
				if (!tdgbl->gbl_sw_blk_factor)
					BURP_error(183, *argv, 0, 0, 0, 0);	/* msg 183 expected blocking factor, encountered "%s"  */
				argv++;
			}
			else if (in_sw_tab->in_sw == IN_SW_BURP_SE) {
				if (argv >= end) {
					BURP_error(273, 0, 0, 0, 0, 0);	/* msg 273: service name parameter missing */
				}
				in_sw_tab->in_sw_state = FALSE;
				++argv;			/* skip a service specification */
			}
			else if (in_sw_tab->in_sw == IN_SW_BURP_D) {
				device = *argv;
				if (argv >= end)	/* device may equal NULL */
					device = NULL;
				else if (*device == '-')
					device = NULL;
				else
					++argv;
			}
			/* want to do output redirect handling now instead of waiting */
			else if (in_sw_tab->in_sw == IN_SW_BURP_Y) {
				redirect = *argv;
				if (argv >= end)	/* redirect may equal NULL */
					redirect = NULL;
				else if (*redirect == '-')
					redirect = NULL;
				else
					++argv;
				if (!redirect)
					BURP_error(4, 0, 0, 0, 0, 0);	/* msg 4 redirect location for output is not specified */

				p = redirect;
				string = OUTPUT_SUPPRESS;
				tdgbl->sw_redirect = NOOUTPUT;
				while (c = *p++) {
					if (UPPER(c) != *string++) {
						tdgbl->sw_redirect = TRUE;
						break;
					}
				}
				if (tdgbl->sw_redirect == TRUE) {	/* not FALSE, and not NOOUTPUT */

					/* Make sure the status file doesn't already exist */
					if (tmp_outfile = ib_fopen(redirect, FOPEN_READ_TYPE)) {
						BURP_print(66, redirect, 0, 0, 0, 0);
						/* msg 66 can't open status and error output file %s */
						ib_fclose(tmp_outfile);
						EXIT(FINI_ERROR);
					}
					if (!
						(tdgbl->output_file =
						 ib_fopen(redirect, FOPEN_WRITE_TYPE))) {
						BURP_print(66, redirect, 0, 0, 0, 0);
						/* msg 66 can't open status and error output file %s */
						EXIT(FINI_ERROR);
					}
				}
			}					/*else if (in_sw_tab->in_sw == IN_SW_BURP_Y) */
		}						/* else */
	}							/* while (argv < end) */

/* reverse the linked list of file blocks */

	tdgbl->gbl_sw_files = NULL;

	for (file = file_list; file; file = next_file) {
		next_file = file->fil_next;
		file->fil_next = tdgbl->gbl_sw_files;
		tdgbl->gbl_sw_files = file;
	}

/* pop off the obviously boring ones, plus do some checking */

	for (file = tdgbl->gbl_sw_files; file; file = file->fil_next) {
		if (!file1)
			file1 = file->fil_name;
		else if (!file2)
			file2 = file->fil_name;
		for (file_list = file->fil_next; file_list;
			 file_list = file_list->fil_next) {
			if (!strcmp(file->fil_name, file_list->fil_name))
				BURP_error(9, 0, 0, 0, 0, 0);	/* msg 9 mutiple sources or destinations specified */
		}

	}

/* Initialize 'dpb' and 'dpb_length' */
	*dpb++ = gds_dpb_version1;
	*dpb++ = isc_dpb_gbak_attach;
	*dpb++ = strlen(GDS_VERSION);
	for (q = GDS_VERSION; *q;)
		*dpb++ = *q++;
	tdgbl->dpb_length = dpb - tdgbl->dpb_string;

	for (in_sw_tab = burp_in_sw_table; in_sw_tab->in_sw_name; in_sw_tab++) {
		if (in_sw_tab->in_sw_state) {
			switch (in_sw_tab->in_sw) {
			case (IN_SW_BURP_B):
				if (sw_replace)
					BURP_error(5, 0, 0, 0, 0, 0);	/* msg 5 conflicting switches for backup/restore */
				sw_replace = IN_SW_BURP_B;
				break;

			case (IN_SW_BURP_C):
				if (sw_replace == IN_SW_BURP_B)
					BURP_error(5, 0, 0, 0, 0, 0);	/* msg 5 conflicting switches for backup/restore */
				if (sw_replace != IN_SW_BURP_R)
					sw_replace = IN_SW_BURP_C;
				break;

			case (IN_SW_BURP_CO):
				tdgbl->gbl_sw_convert_ext_tables = TRUE;
				break;

			case (IN_SW_BURP_E):
				tdgbl->gbl_sw_compress = FALSE;
				break;

			case (IN_SW_BURP_G):
				if (!tdgbl->dpb_length)
					*dpb++ = gds_dpb_version1;
				*dpb++ = gds_dpb_no_garbage_collect;
				*dpb++ = 0;
				tdgbl->dpb_length = dpb - tdgbl->dpb_string;
				break;

			case (IN_SW_BURP_I):
				tdgbl->gbl_sw_deactivate_indexes = TRUE;
				break;

			case (IN_SW_BURP_IG):
				if (!tdgbl->dpb_length)
					*dpb++ = gds_dpb_version1;
				*dpb++ = gds_dpb_damaged;
				*dpb++ = 1;
				*dpb++ = 1;
				tdgbl->dpb_length = dpb - tdgbl->dpb_string;
				break;

			case (IN_SW_BURP_K):
				tdgbl->gbl_sw_kill = TRUE;
				break;

			case (IN_SW_BURP_L):
				tdgbl->gbl_sw_ignore_limbo = TRUE;
				break;

			case (IN_SW_BURP_M):
				tdgbl->gbl_sw_meta = TRUE;
				break;

			case (IN_SW_BURP_MODE):
				tdgbl->gbl_sw_mode = TRUE;
				break;

			case (IN_SW_BURP_N):
				tdgbl->gbl_sw_novalidity = TRUE;
				break;

			case (IN_SW_BURP_NT):	/* Backup non-transportable format */
				tdgbl->gbl_sw_transportable = FALSE;
				break;

			case (IN_SW_BURP_O):
				tdgbl->gbl_sw_incremental = TRUE;
				break;

			case (IN_SW_BURP_OL):
				tdgbl->gbl_sw_old_descriptions = TRUE;
				break;

			case (IN_SW_BURP_PASS):
				if (!tdgbl->dpb_length)
					*dpb++ = gds_dpb_version1;
				if (!tdgbl->gbl_sw_service_gbak)
					*dpb++ = gds_dpb_password;
				else
					*dpb++ = gds_dpb_password_enc;
				*dpb++ = strlen(tdgbl->gbl_sw_password);
				for (q = tdgbl->gbl_sw_password; *q;)
					*dpb++ = *q++;
				tdgbl->dpb_length = dpb - tdgbl->dpb_string;
				break;

			case (IN_SW_BURP_R):
				if (sw_replace == IN_SW_BURP_B)
					BURP_error(5, 0, 0, 0, 0, 0);	/* msg 5 conflicting switches for backup/restore */
				sw_replace = IN_SW_BURP_R;
				break;

			case (IN_SW_BURP_T):
				tdgbl->gbl_sw_transportable = TRUE;
				break;

			case (IN_SW_BURP_U):
				BURP_error(7, 0, 0, 0, 0, 0);	/* msg 7 protection isn't there yet */
				break;

			case (IN_SW_BURP_US):
				tdgbl->gbl_sw_no_reserve = TRUE;
				break;

			case (IN_SW_BURP_ROLE):
				if (!tdgbl->dpb_length)
					*dpb++ = isc_dpb_version1;
				*dpb++ = isc_dpb_sql_role_name;
				*dpb++ = strlen(tdgbl->gbl_sw_sql_role);
				for (q = tdgbl->gbl_sw_sql_role; *q;)
					*dpb++ = *q++;
				tdgbl->dpb_length = dpb - tdgbl->dpb_string;
				break;

			case (IN_SW_BURP_USER):
				if (!tdgbl->dpb_length)
					*dpb++ = gds_dpb_version1;
				*dpb++ = gds_dpb_user_name;
				*dpb++ = strlen(tdgbl->gbl_sw_user);
				for (q = tdgbl->gbl_sw_user; *q;)
					*dpb++ = *q++;
				tdgbl->dpb_length = dpb - tdgbl->dpb_string;
				break;

			case (IN_SW_BURP_V):
				tdgbl->gbl_sw_verbose = TRUE;
				break;

			case (IN_SW_BURP_Z):
				BURP_print(91, (void*) GDS_VERSION, 0, 0, 0, 0);	/* msg 91 gbak version %s */
				tdgbl->gbl_sw_version = TRUE;
				break;

			default:
				break;
			}
		}
	}

	if (!sw_replace)
		sw_replace = IN_SW_BURP_B;

	if (tdgbl->gbl_sw_page_size)
	{
		if (sw_replace == IN_SW_BURP_B)
			BURP_error(8, 0, 0, 0, 0, 0);	/* msg 8 page size is allowed only on restore or create */
		temp = tdgbl->gbl_sw_page_size;
		{
			int curr_pg_size = 1024;
			while (curr_pg_size <= MAX_PAGE_SIZE) {
				if (temp <= curr_pg_size) {
					temp = curr_pg_size;
					break;
				}
				curr_pg_size <<= 1;
			}
		}
		if (temp > MAX_PAGE_SIZE)
		{
#ifdef SUPERSERVER
			BURP_svc_error(	3,
							isc_arg_number,
							reinterpret_cast<void*>(tdgbl->gbl_sw_page_size),
							0, NULL, 0, NULL,
							0, NULL, 0, NULL);
#else
			/* msg 3 Page size specified (%ld) greater than limit (MAX_PAGE_SIZE bytes) */
			BURP_error(3, (void*) (ULONG) tdgbl->gbl_sw_page_size, 0, 0, 0, 0);
#endif
		}
		if (temp != tdgbl->gbl_sw_page_size) {
			BURP_print(103, (TEXT *) (ULONG) tdgbl->gbl_sw_page_size
					   , (TEXT *) (SLONG) temp,
					   0, 0, 0);	/* msg 103 page size specified (%ld bytes) rounded up to %ld bytes */
			tdgbl->gbl_sw_page_size = temp;
		}
	}

	if (tdgbl->gbl_sw_page_buffers) {
		if (sw_replace == IN_SW_BURP_B)
			BURP_error(260, 0, 0, 0, 0, 0);	/* msg 260 page buffers is allowed only on restore or create */
	}

	if (!tdgbl->gbl_sw_blk_factor || sw_replace != IN_SW_BURP_B)
		tdgbl->gbl_sw_blk_factor = 1;

	if (!file2)
		BURP_error(10, 0, 0, 0, 0, 0);	/* msg 10 requires both input and output filenames */

	if (!strcmp(file1, file2))
		BURP_error(11, 0, 0, 0, 0, 0);	/* msg 11 input and output have the same name.  Disallowed. */

	clock = time(NULL);
	strcpy(const_cast<char*>(tdgbl->gbl_backup_start_time), ctime(&clock));
	p =
		const_cast<char*>(tdgbl->gbl_backup_start_time +
				strlen(const_cast<const char*>(tdgbl->gbl_backup_start_time)) - 1);
	if (*p == '\n')
		*p = 0;

	tdgbl->action = (ACT) BURP_ALLOC_ZERO(ACT_LEN);
	tdgbl->action->act_total = 0;
	tdgbl->action->act_file = NULL;
	tdgbl->action->act_action = ACT_unknown;

	action =
		open_files(file1, &file2, tdgbl->gbl_sw_verbose, sw_replace, sw_tape);

	MVOL_init(tdgbl->io_buffer_size);

	switch (action) {
	case (RESTORE):
		SVC_STARTED(tdgbl->service_blk);
		result = RESTORE_restore(file1, file2);
		break;

	case (BACKUP):
		SVC_STARTED(tdgbl->service_blk);
		result = BACKUP_backup(file1, file2);
		break;

	case (QUIT):
		SVC_STARTED(tdgbl->service_blk);
		BURP_abort();
		break;
	}
	if (result != FINI_OK && result != FINI_DB_NOT_ONLINE)
		BURP_abort();

	EXIT(result);
	return result;
	}	// try

	catch (const std::exception&)
	{
		int exit_code;
		UCHAR *mem;

		/* All calls to EXIT(), normal and error exits, wind up here */

		tdgbl->burp_env = NULL;
		exit_code = tdgbl->exit_code;

		/* Close the gbak file handles if they still open */
		for (file = tdgbl->gbl_sw_backup_files; file; file = file->fil_next) {
			if (file->fil_fd != INVALID_HANDLE_VALUE)
				CLOSE(file->fil_fd);
			if (exit_code != 0
				&& (tdgbl->action->act_action == ACT_backup_split
					|| tdgbl->action->act_action == ACT_backup))
				UNLINK(file->fil_name);
		}

		/* Detach from database to release system resources */
		if (tdgbl->db_handle != 0) {
			close_out_transaction(action,
								  const_cast<isc_tr_handle*>(&tdgbl->tr_handle));
			close_out_transaction(action,
								  const_cast<isc_tr_handle*>(&tdgbl->global_trans));
			if (isc_detach_database(const_cast<ISC_STATUS*>(tdgbl->status_vector),
									const_cast<isc_db_handle*>(&tdgbl->db_handle)))
			{
				BURP_print_status(const_cast<ISC_STATUS*>(tdgbl->status_vector));
			}
		}

		/* Close the status output file */
		if (tdgbl->sw_redirect == TRUE && tdgbl->output_file != NULL) {
			ib_fclose(tdgbl->output_file);
			tdgbl->output_file = NULL;
		}

		/* Free all unfreed memory used by Gbak itself */
		while (tdgbl->head_of_mem_list != NULL) {
			mem = tdgbl->head_of_mem_list;
			tdgbl->head_of_mem_list = *((UCHAR **) tdgbl->head_of_mem_list);
			gds__free(mem);
		}

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
}



void BURP_abort(void)
{
/**************************************
 *
 *	B U R P _ a b o r t
 *
 **************************************
 *
 * Functional description
 *	Abandon a failed operation.
 *
 **************************************/
	TGBL tdgbl;

	tdgbl = GET_THREAD_DATA;

	BURP_print(83, 0, 0, 0, 0, 0);	/* msg 83 Exiting before completion due to errors */

#ifdef SUPERSERVER
	SVC_STARTED(tdgbl->service_blk);
#endif

	EXIT(FINI_ERROR);
}


void BURP_error(USHORT errcode,
				const void* arg1,
				const void* arg2,
				const void* arg3,
				const void* arg4,
				const void* arg5)
{
/**************************************
 *
 *	 B U R P _ e r r o r
 *
 **************************************
 *
 * Functional description
 *	Format and print an error message, then punt.
 *
 **************************************/
#ifdef SUPERSERVER
	TGBL tdgbl;
	ISC_STATUS *status;

	tdgbl = GET_THREAD_DATA;
	status = tdgbl->service_blk->svc_status;

	CMD_UTIL_put_svc_status(status, BURP_MSG_FAC, errcode,
							isc_arg_string, arg1,
							isc_arg_string, arg2,
							isc_arg_string, arg3,
							isc_arg_string, arg4, isc_arg_string, arg5);
	SVC_STARTED(tdgbl->service_blk);
#endif

	BURP_msg_partial(256, 0, 0, 0, 0, 0);	/* msg 256: gbak: ERROR: */
	BURP_msg_put(errcode, arg1, arg2, arg3, arg4, arg5);
	BURP_abort();
}


void BURP_print_status( ISC_STATUS * status_vector)
{
/**************************************
 *
 *	B U R P _ p r i n t _ s t a t u s
 *
 **************************************
 *
 * Functional description
 *	Print error message. Use isc_interprete
 *	to allow redirecting output.
 *
 **************************************/
#ifdef SUPERSERVER
	TGBL tdgbl;
	ISC_STATUS *status;
	int i = 0, j;
#endif
	ISC_STATUS *vector;
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

		if (isc_interprete(s, &vector)) {
			TRANSLATE_CP(s);
			BURP_msg_partial(256, 0, 0, 0, 0, 0);	/* msg 256: gbak: ERROR: */
			burp_output("%s\n", s);
			while (isc_interprete(s, &vector)) {
				TRANSLATE_CP(s);
				BURP_msg_partial(256, 0, 0, 0, 0, 0);	/* msg 256: gbak: ERROR: */
				burp_output("    %s\n", s);
			}
		}
	}
}


void BURP_print_warning( ISC_STATUS * status_vector)
{
/**************************************
 *
 *	B U R P _ p r i n t _ w a r n i n g
 *
 **************************************
 *
 * Functional description
 *	Print warning message. Use isc_interprete
 *	to allow redirecting output.
 *
 **************************************/
	ISC_STATUS *vector;
	SCHAR s[1024];

	if (status_vector) {
		/* skip the error, assert that one does not exist */
		assert(status_vector[0] == gds_arg_gds);
		assert(status_vector[1] == 0);
		/* print the warning message */
		vector = &status_vector[2];
		if (isc_interprete(s, &vector)) {
			TRANSLATE_CP(s);
			BURP_msg_partial(255, 0, 0, 0, 0, 0);	/* msg 255: gbak: WARNING: */
			burp_output("%s\n", s);
			while (isc_interprete(s, &vector)) {
				TRANSLATE_CP(s);
				BURP_msg_partial(255, 0, 0, 0, 0, 0);	/* msg 255: gbak: WARNING: */
				burp_output("    %s\n", s);
			}
		}
	}
}


void BURP_error_redirect(	ISC_STATUS* status_vector,
							USHORT errcode,
							void* arg1,
							void* arg2)
{
/**************************************
 *
 *	B U R P _ e r r o r _ r e d i r e c t
 *
 **************************************
 *
 * Functional description
 *	Issue error message. Output messages then abort.
 *
 **************************************/

	BURP_print_status(status_vector);
	BURP_error(errcode, arg1, arg2, NULL, NULL, NULL);
}


void BURP_msg_partial(	USHORT number,
						const void* arg1,
						const void* arg2,
						const void* arg3,
						const void* arg4,
						const void* arg5)
{
/**************************************
 *
 *	B U R P _ m s g _ p a r t i a l
 *
 **************************************
 *
 * Functional description
 *	Retrieve a message from the error file,
 *	format it, and print it without a newline.
 *
 **************************************/
	TEXT buffer[256];

	gds__msg_format(NULL,
					BURP_MSG_FAC,
					number,
					sizeof(buffer),
					buffer,
					static_cast<const char*>(arg1),
					static_cast<const char*>(arg2),
					static_cast<const char*>(arg3),
					static_cast<const char*>(arg4),
					static_cast<const char*>(arg5));
	burp_output("%s", buffer);
}


void BURP_msg_put(	USHORT number,
					const void* arg1,
					const void* arg2,
					const void* arg3,
					const void* arg4,
					const void* arg5)
{
/**************************************
 *
 *	B U R P _ m s g _ p u t
 *
 **************************************
 *
 * Functional description
 *	Retrieve a message from the error file, format it, and print it.
 *
 **************************************/
	TEXT buffer[256];

	gds__msg_format(NULL,
					BURP_MSG_FAC,
					number,
					sizeof(buffer),
					buffer,
					static_cast<const char*>(arg1),
					static_cast<const char*>(arg2),
					static_cast<const char*>(arg3),
					static_cast<const char*>(arg4),
					static_cast<const char*>(arg5));
	TRANSLATE_CP(buffer);
	burp_output("%s\n", buffer);
}


void BURP_msg_get(	USHORT number,
					void* msg,
					void* arg1,
					void* arg2,
					void* arg3,
					void* arg4,
					void* arg5)
{
/**************************************
 *
 *	B U R P _ m s g _ g e t
 *
 **************************************
 *
 * Functional description
 *	Retrieve a message from the error file, format it and copy it to the buffer
 *
 **************************************/
	TEXT buffer[128];

	gds__msg_format(NULL,
					BURP_MSG_FAC,
					number,
					sizeof(buffer),
					buffer,
					reinterpret_cast<char*>(arg1),
					reinterpret_cast<char*>(arg2),
					reinterpret_cast<char*>(arg3),
					reinterpret_cast<char*>(arg4),
					reinterpret_cast<char*>(arg5));
	strcpy(reinterpret_cast<char*>(msg), buffer);
}


void BURP_output_version( TEXT * arg1, TEXT * arg2)
{
/**************************************
 *
 *	B U R P _ o u t p u t _ v e r s i o n
 *
 **************************************
 *
 * Functional description
 *	Callback routine for access method
 *	printing (specifically show version);
 *	will accept.
 *
 **************************************/

	burp_output(arg1, arg2);
}


void BURP_print(USHORT number,
				const void* arg1,
				const void* arg2,
				const void* arg3,
				const void* arg4,
				const void* arg5)
{
/**************************************
 *
 *	B U R P _ p r i n t
 *
 **************************************
 *
 * Functional description
 *	Display a formatted error message
 *	in a way that VMS or civilized systems
 *	will accept.
 *
 **************************************/

	BURP_msg_partial(169, 0, 0, 0, 0, 0);	/* msg 169: gbak: */
	BURP_msg_put(number, arg1, arg2, arg3, arg4, arg5);
}


void BURP_verbose(USHORT number,
				  const void* arg1,
				  const void* arg2,
				  const void* arg3,
				  const void* arg4,
				  const void* arg5)
{
/**************************************
 *
 *	B U R P _ v e r b o s e
 *
 **************************************
 *
 * Functional description
 *	Calls BURP_print for displaying a formatted error message
 *	but only for verbose output.  If not verbose then calls
 *	user defined yieding function.
 *
 **************************************/

	TGBL tdgbl = GET_THREAD_DATA;

	if (tdgbl->gbl_sw_verbose)
		BURP_print(number, arg1, arg2, arg3, arg4, arg5);
	else
		burp_output("%s","");
}

}	// extern "C"


static void close_out_transaction(volatile  SSHORT action,
								  isc_tr_handle* handle)
{
/**************************************
 *
 *	c l o s e _ o u t _ t r a n s a c t i o n
 *
 **************************************
 *
 * Functional description
 *	Make a transaction go away.  This is
 *	important as we need to detach from the
 *	database so all engine allocated memory is
 *	returned to the system.
 *
 **************************************/
	ISC_STATUS_ARRAY status_vector;

	if (*handle != 0) {
		if (action == RESTORE) {
			/* Even if the restore failed, commit the transaction so that
			 * a partial database is at least recovered.
			 */
			isc_commit_transaction(status_vector, handle);
			if (status_vector[1]) {
				/* If we can't commit - have to roll it back, as
				 * we need to close all outstanding transactions before
				 * we can detach from the database.
				 */
				isc_rollback_transaction(status_vector, handle);
				if (status_vector[1])
					BURP_print_status(status_vector);
			}
		}
		else
			/* A backup shouldn't touch any data - we ensure that
			 * by never writing data during a backup, but let's double
			 * ensure it by doing a rollback
			 */
		if (isc_rollback_transaction(status_vector, handle))
			BURP_print_status(status_vector);
	}
}


static SLONG get_number( SCHAR * string)
{
/**************************************
 *
 *	g e t _ n u m b e r
 *
 **************************************
 *
 * Functional description
 *	Convert a string to binary, complaining bitterly if
 *	the string is bum.
 *
 **************************************/
	SCHAR c, *p;
	SLONG value;

	for (value = 0, p = string; c = *p++;) {
		if (c < '0' || c > '9')
			return 0;
		value *= 10;
		value += c - '0';
	}

	return value;
}


static SSHORT open_files(TEXT * file1,
						 TEXT ** file2,
						 USHORT sw_verbose, USHORT sw_replace, USHORT sw_tape)
{
/**************************************
 *
 *	o p e n _ f i l e s
 *
 **************************************
 *
 * Functional description
 *	from the input file names and
 *	positions, guess what the users
 *	intention was.  Return the type
 *	of the first file, plus open file
 *	and db handle.
 *
 **************************************/
	ISC_STATUS *status_vector;
	TGBL tdgbl;
	FIL fil;
	int seq, total;
	SSHORT flag;


	tdgbl = GET_THREAD_DATA;
	status_vector = tdgbl->status;

/* try to attach the database using the first file_name */

	if (sw_replace != IN_SW_BURP_C && sw_replace != IN_SW_BURP_R)
		if (!(isc_attach_database(status_vector,
								  (SSHORT) 0,
								  file1,
								  &tdgbl->db_handle,
								  tdgbl->dpb_length,
								  reinterpret_cast<char*>(tdgbl->dpb_string))))
		{
			if (sw_replace != IN_SW_BURP_B) {
				/* msg 13 REPLACE specified, but the first file %s is a database */
				BURP_error(13, file1, 0, 0, 0, 0);
				if (isc_detach_database(status_vector, &tdgbl->db_handle)) {
					BURP_print_status(status_vector);
				}
				return QUIT;
			}
			if (tdgbl->gbl_sw_version) {
				/* msg 139 Version(s) for database "%s" */
				BURP_print(139, file1, 0, 0, 0, 0);
				isc_version(&tdgbl->db_handle,
							reinterpret_cast<void (*)()>(BURP_output_version),
							(void*) "\t%s\n");
			}
			if (sw_verbose)
				BURP_print(166, file1, 0, 0, 0, 0);	/* msg 166: readied database %s for backup */
		}
		else if (sw_replace == IN_SW_BURP_B ||
				 (status_vector[1] != gds_io_error
				  && status_vector[1] != gds_bad_db_format)) {
			BURP_print_status(status_vector);
			return QUIT;
		}

	if (sw_replace == IN_SW_BURP_B) {

		/* Now it is safe to skip a db file */
		tdgbl->gbl_sw_backup_files = tdgbl->gbl_sw_files->fil_next;
		tdgbl->gbl_sw_files = tdgbl->gbl_sw_files->fil_next;
		assert(strcmp(tdgbl->gbl_sw_files->fil_name, *file2) == 0);

		flag = BACKUP;
		tdgbl->action->act_action = ACT_backup;
		for (fil = tdgbl->gbl_sw_files; fil; fil = fil->fil_next)
		{
			/* adjust the file size first */
			switch (fil->fil_size_code)
			{
			case size_n:
				break;
			case size_k:
				fil->fil_length *= KBYTE;
				break;
			case size_m:
				fil->fil_length *= MBYTE;
				break;
			case size_g:
				fil->fil_length *= GBYTE;
				break;
			case size_e:
				BURP_error(262, fil->fil_name, 0, 0, 0, 0);	/* msg 262 size specification either missing or incorrect for file %s  */
				break;
			default:
				assert(FALSE);
				break;
			}

			if ((fil->fil_seq = ++(tdgbl->action->act_total)) >= 2)
			{
				tdgbl->action->act_action = ACT_backup_split;
			}
			if (sw_verbose)
			{
				BURP_print(75, fil->fil_name, 0, 0, 0, 0);	/* msg 75  creating file %s */
			}
			if (!strcmp(fil->fil_name, "stdout"))
			{
				if (tdgbl->action->act_total >= 2 || fil->fil_next)
				{
					BURP_error(266, 0, 0, 0, 0, 0);	/* msg 266 standard output is not supported when using split operation */
					flag = QUIT;
					break;
				}
				/* We ignore SIGPIPE so that we can report an IO error when we
				 * try to write to the broken pipe.
				 */
#ifndef WIN_NT
				signal(SIGPIPE, SIG_IGN);
#endif
				fil->fil_fd = reinterpret_cast<DESC>(GBAK_STDOUT_DESC);
				break;
			}
			else
			{

#ifdef WIN_NT
				if ((fil->fil_fd = MVOL_open(fil->fil_name, MODE_WRITE,
											 CREATE_ALWAYS)) == INVALID_HANDLE_VALUE)
#else
				if ((fil->fil_fd = open(fil->fil_name, MODE_WRITE, OPEN_MASK)) == -1)
#endif /* WIN_NT */

				{

#ifdef SUPERSERVER
					BURP_svc_error(65, isc_arg_string, fil->fil_name,
								   0, NULL, 0, NULL, 0, NULL, 0, NULL);
#endif
					BURP_print(65, fil->fil_name, 0, 0, 0, 0);	/* msg 65 can't
																   open backup file %s */
					flag = QUIT;
					break;
				}
			}

			if (fil->fil_length == 0)
			{
				if (fil->fil_next)
				{
					BURP_error(262, fil->fil_name, 0, 0, 0, 0);	/* msg 262 size specification either missing or incorrect for file %s  */
					flag = QUIT;
					break;
				}
				else
				{
					fil->fil_length = MAX_LENGTH;	/* Write as much as possible to
													   the last file */
				}
			}
			if (fil->fil_length < MIN_SPLIT_SIZE)
			{
#ifdef SUPERSERVER
				BURP_svc_error(	271,
								isc_arg_number,
								reinterpret_cast<void*>(fil->fil_length),
								isc_arg_number,
								reinterpret_cast<void*>(MIN_SPLIT_SIZE),
								0, NULL, 0, NULL, 0, NULL);
#else
				BURP_error(271, (TEXT *) fil->fil_length,
						   (TEXT *) (SLONG) MIN_SPLIT_SIZE, 0, 0, 0);
				/* msg file size given (%d) is less than minimum allowed (%d) */
#endif
				flag = QUIT;
				break;
			}
		}

		if (flag == BACKUP)
		{
			tdgbl->action->act_file = tdgbl->gbl_sw_files;
			tdgbl->file_desc = tdgbl->gbl_sw_files->fil_fd;
		}
		else
		{
			if (isc_detach_database(status_vector, &tdgbl->db_handle))
			{
				BURP_print_status(status_vector);
			}
		}

		return flag;
	}


/*
 * If we got to here, then we're really not backing up a database,
 * so open a backup file.
 */

/* There are four possible cases such as:
 *
 *   1. restore single backup file to single db file
 *   2. restore single backup file to multiple db files
 *   3. restore multiple backup files (join operation) to single db file
 *   4. restore multiple backup files (join operation) to multiple db files
 *
 * Just looking at the command line, we can't say for sure whether it is a
 * specification of the last file to be join or it is a specification of the
 * primary db file (case 4), for example:
 *
 *     gbak -c gbk1 gbk2 gbk3 db1 200 db2 500 db3 -v
 *                            ^^^
 *     db1 could be either the last file to be join or primary db file
 *
 * Since 'gbk' and 'gsplit' formats are different (gsplit file has its own
 * header record) hence we can use it as follows:
 *
 *     - open first file
 *     - read & check a header record
 *
 *    If a header is identified as a 'gsplit' one then we know exactly how
 * many files need to be join and in which order. We keep opening a file by
 * file till we reach the last one to be join. During this step we check
 * that the files are accessible and are in proper order. It gives us
 * possibility to let silly customer know about an error as soon as possible.
 * Besides we have to find out which file is going to be a db file.
 *
 *    If header is not identified as a 'gsplit' record then we assume that
 * we got a single backup file.
 */

	fil = tdgbl->gbl_sw_files;
	tdgbl->gbl_sw_backup_files = tdgbl->gbl_sw_files;

	tdgbl->action->act_action = ACT_restore;
	if (!strcmp(fil->fil_name, "stdin")) {
		fil->fil_fd = reinterpret_cast<DESC>(GBAK_STDIN_DESC);
		tdgbl->file_desc = fil->fil_fd;
		tdgbl->gbl_sw_files = fil->fil_next;
	}
	else {
		/* open first file */
#ifdef WIN_NT
		if ((fil->fil_fd = MVOL_open(fil->fil_name, MODE_READ, OPEN_EXISTING))
			== INVALID_HANDLE_VALUE)
#else
		if ((fil->fil_fd = open(fil->fil_name, MODE_READ)) ==
			INVALID_HANDLE_VALUE)
#endif
		{
			BURP_error(65, fil->fil_name, 0, 0, 0, 0);	/* msg 65 can't open backup file %s */
			return QUIT;
		}

		if (sw_verbose)
			BURP_print(100, fil->fil_name, 0, 0, 0, 0);	/* msg 100 opened file
														   %s */
		/* read and check a header record */
		tdgbl->action->act_file = fil;
		seq = 1;
		if (MVOL_split_hdr_read() == TRUE) {
			tdgbl->action->act_action = ACT_restore_join;
			total = tdgbl->action->act_total;	/* number of files to be join */
			if (fil->fil_seq != seq || seq > total) {
				BURP_error(263, fil->fil_name, 0, 0, 0, 0);	/* msg 263 file %s out of sequence */
				return QUIT;
			}

			for (++seq, fil = fil->fil_next; seq <= total;
				 fil = fil->fil_next, seq++) {
				if (!fil) {
					BURP_error(264, 0, 0, 0, 0, 0);	/* msg 264 can't join -- one of the files missing */
					return QUIT;
				}
				if (!strcmp(fil->fil_name, "stdin")) {
					BURP_error(265, 0, 0, 0, 0, 0);	/* msg 265 standard input is not supported when using join operation */
					return QUIT;
				}
				tdgbl->action->act_file = fil;
#ifdef WIN_NT
				if ((fil->fil_fd = MVOL_open(fil->fil_name, MODE_READ,
											 OPEN_EXISTING)) ==
					INVALID_HANDLE_VALUE)
#else
				if ((fil->fil_fd = open(fil->fil_name, MODE_READ))
					== INVALID_HANDLE_VALUE)
#endif
				{
#ifdef SUPERSERVER
					BURP_svc_error(65, isc_arg_string, fil->fil_name,
								   0, NULL, 0, NULL, 0, NULL, 0, NULL);
#endif
					BURP_print(65, fil->fil_name, 0, 0, 0, 0);	/* msg 65 can't open
																   backup file %s */
					return QUIT;
				}

				if (sw_verbose)
					BURP_print(100, fil->fil_name, 0, 0, 0, 0);	/* msg 100
																   opened file
																   %s */
				if (MVOL_split_hdr_read() == TRUE) {
					if ((total != tdgbl->action->act_total) ||
						(seq != fil->fil_seq) || (seq > total)) {
						BURP_error(263, fil->fil_name, 0, 0, 0, 0);	/* msg 263 file %s out of sequence */
						return QUIT;
					}
				}
				else {
					BURP_error(267, fil->fil_name, 0, 0, 0, 0);	/* msg 267 backup file %s might be corrupt */
					return QUIT;
				}
			}
			tdgbl->action->act_file = tdgbl->gbl_sw_files;
			tdgbl->file_desc = tdgbl->action->act_file->fil_fd;
			if ((tdgbl->gbl_sw_files = fil) == NULL) {
				BURP_error(268, 0, 0, 0, 0, 0);	/* msg 268 database file specification missing */
				return QUIT;
			}
		}
		else {
			/* Move pointer to the begining of the file. At this point we
			   assume -- this is a single backup file because we were
			   not able to read a split header record.
			 */
#ifdef WIN_NT
			if (strnicmp(fil->fil_name, "\\\\.\\tape", 8))
				SetFilePointer(fil->fil_fd, 0, NULL, FILE_BEGIN);
			else
				SetTapePosition(fil->fil_fd, TAPE_REWIND, 0, 0, 0, FALSE);
#else
			lseek(fil->fil_fd, 0, SEEK_SET);
#endif
			tdgbl->file_desc = fil->fil_fd;
			tdgbl->gbl_sw_files = fil->fil_next;
		}
	}


/* If we got here, we've opened a backup file, and we're
   thinking about creating or replacing a database. */

	*file2 = tdgbl->gbl_sw_files->fil_name;
	if (tdgbl->gbl_sw_files->fil_size_code != size_n)
		BURP_error(262, *file2, 0, 0, 0, 0);	/* msg 262 size specificati on either missing or incorrect for file %s  */

	if ((sw_replace == IN_SW_BURP_C || sw_replace == IN_SW_BURP_R) &&
		!isc_attach_database(status_vector,
							 (SSHORT) 0,
							 *file2,
							 &tdgbl->db_handle,
							 tdgbl->dpb_length,
							 reinterpret_cast<char*>(tdgbl->dpb_string)))
	{
		if (sw_replace == IN_SW_BURP_C) {
			if (isc_detach_database(status_vector, &tdgbl->db_handle)) {
				BURP_print_status(status_vector);
			}
			BURP_error(14, *file2, 0, 0, 0, 0);
			/* msg 14 database %s already exists.  To replace it, use the -R switch */
		}
		else {
			isc_drop_database(status_vector, &tdgbl->db_handle);
			if (tdgbl->db_handle) {
				ISC_STATUS_ARRAY status_vector2;
				if (isc_detach_database(status_vector2, &tdgbl->db_handle)) {
					BURP_print_status(status_vector2);
				}

				/* Complain only if the drop database entrypoint is available.
				   If it isn't, the database will simply be overwritten. */

				if (status_vector[1] != gds_unavailable)
					BURP_error(233, *file2, 0, 0, 0, 0);
				/* msg 233 Cannot drop database %s, might be in use */
			}
		}
	}
	if (sw_replace == IN_SW_BURP_R && status_vector[1] == isc_adm_task_denied) {
		/* if we got an error from attach database and we have replace switch set
		 * then look for error from attach returned due to not owner, if we are
		 * not owner then return the error status back up
		 */
		BURP_error(274, 0, 0, 0, 0, 0);
		/* msg # 274 : Cannot restore over current database, must be sysdba
		   * or owner of the existing database.
		 */
	}
/* if we got here, then all is well, remove any error condition from the
 * status vector when running as a service thread.  If we don't then the
 * service will think that there is an error if isc_attach_database failed
 * like it should have (if creating a database).
 */
	if (tdgbl->gbl_sw_service_thd)
		memset(tdgbl->status, 0, ISC_STATUS_LENGTH * sizeof(ISC_STATUS));

/* check the file size specification */
	for (fil = tdgbl->gbl_sw_files; fil; fil = fil->fil_next) {
		if (fil->fil_size_code != size_n)
			BURP_error(262, fil->fil_name, 0, 0, 0, 0);	/* msg 262 size specification either missing or incorrect for file %s  */
	}

	return RESTORE;
}


static void burp_output( const SCHAR * format, ...)
{
/**************************************
 *
 *	b u r p _ o u t p u t
 *
 **************************************
 *
 * Functional description
 *	Platform independent output routine.
 *
 **************************************/
	va_list arglist;
	UCHAR buf[1000];
	int exit_code;
	TGBL tdgbl;

	tdgbl = GET_THREAD_DATA;

	if (tdgbl->sw_redirect == NOOUTPUT || format[0] == '\0') {
		exit_code =
			tdgbl->output_proc(tdgbl->output_data,
							   (UCHAR*)(""));
	}
	else if (tdgbl->sw_redirect == TRUE && tdgbl->output_file != NULL) {
		VA_START(arglist, format);
		ib_vfprintf(tdgbl->output_file, format, arglist);
		va_end(arglist);
		exit_code =
			tdgbl->output_proc(tdgbl->output_data,
							   (UCHAR*)(""));
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


static ULONG get_size( SCHAR * string, FIL file)
{
/**********************************************
 *
 *      g e t _ s i z e
 *
 **********************************************
 *
 * Functional description
 *	Get size specification for either splitting or
 *	restoring to multiple files
 *
 **********************************************/
	SCHAR *num, c;
	ULONG size;
	BOOLEAN digit;


	file->fil_size_code = size_n;
	for (size = 0, digit = FALSE, num = string; c = *num++;) {
		if (isdigit(c)) {
			size = size * 10 + (c - '0');
			digit = TRUE;
		}
		else {
			if (isalpha(c)) {
				if (!digit) {
					file->fil_size_code = size_e;
					size = 0;
					break;
				}
				switch (UPPER(c)) {
				case 'K':
					file->fil_size_code = size_k;
					break;
				case 'M':
					file->fil_size_code = size_m;
					break;
				case 'G':
					file->fil_size_code = size_g;
					break;
				default:
					file->fil_size_code = size_e;
					size = 0;
					break;
				}
				if (*num) {
					file->fil_size_code = size_e;
					size = 0;
				}
				break;
			}
		}
	}

	return (file->fil_length = size);
}


#ifndef SUPERSERVER
static int api_gbak(int argc,
					char *argv[],
					USHORT length,
					TEXT * password,
					TEXT * user,
					TEXT * service,
					BOOLEAN restore,
					BOOLEAN verbose)
{
/**********************************************
 *
 *	a p i _ g b a k
 *
 **********************************************
 *
 * Functional description
 *	Run gbak using services APIs
 *
 **********************************************/
	ISC_STATUS_ARRAY status;
	TEXT **begin, **end, *x, *p, *usr, *pswd;
	USHORT spblen, thdlen;
	char sendbuf[] = { isc_info_svc_line };
	char respbuf[1024];
	FRBRD *svc_handle = NULL;
	char *spb_ptr, *spb, *svc_name, *thd_ptr, *thd;
	struct tgbl *tdgbl, ldgbl;

	tdgbl = &ldgbl;
	SET_THREAD_DATA;
	memset((void *) tdgbl, 0, sizeof(*tdgbl));
	tdgbl->output_proc = output_main;

	if (!user)
		usr = getenv("ISC_USER");
	else
		usr = user;

	if (!password)
		pswd = getenv("ISC_PASSWORD");
	else
		pswd = password;

	spb = (char *) gds__alloc((SLONG) (2 + 2 + ((usr) ? strlen(usr) : 0) +
									   2 + ((pswd) ? strlen(pswd) : 0)) +
									   2 + length);
	/* 'isc_spb_version'
	   'isc_spb_current_version'
	   'isc_spb_user_name'
	   'length'
	   "usr"
	   'isc_spb_password'
	   'length'
	   "pswd"
	   'isc_spb_options'
	   'length'
	   "options" */
	if (spb == NULL) {
		status[0] = isc_arg_gds;
		status[1] = isc_virmemexh;
		status[2] = isc_arg_end;
		BURP_print_status(status);
		BURP_print(83, 0, 0, 0, 0, 0);	/* msg 83 Exiting before completion due to errors */
		return FINI_ERROR;
	}

	spb_ptr = spb;
	*spb_ptr++ = isc_spb_version;
	*spb_ptr++ = isc_spb_current_version;

	if (usr) {
		*spb_ptr++ = isc_spb_user_name;
		*spb_ptr++ = strlen(usr);
		MEMMOVE(usr, spb_ptr, strlen(usr));
		spb_ptr += strlen(usr);
		if (user)
			*user = '\0';
	}

	if (pswd) {
		*spb_ptr++ = isc_spb_password;
		*spb_ptr++ = strlen(pswd);
		MEMMOVE(pswd, spb_ptr, strlen(pswd));
		spb_ptr += strlen(pswd);
		if (password)
			*password = '\0';
	}

	svc_name = (char *) gds__alloc((SLONG) (strlen(service) + 1));

	if (svc_name == NULL) {
		status[0] = isc_arg_gds;
		status[1] = isc_virmemexh;
		status[2] = isc_arg_end;
		BURP_print_status(status);
		gds__free(spb);
		BURP_print(83, 0, 0, 0, 0, 0);	/* msg 83 Exiting before completion due to errors */
		return FINI_ERROR;
	}

	if (service) {
		strcpy(svc_name, service);
		*service = '\0';
	}


/* Fill command line options */

	*spb_ptr++ = isc_spb_command_line;
	begin = argv, end = argv + argc;
	argv++;

	*spb_ptr++ = length;
	while (argv < end) {
		if (**argv && argv > begin + 1)
			*spb_ptr++ = ' ';
		for (x = *argv++; *x;)
			*spb_ptr++ = *x++;
	}

	spblen = spb_ptr - spb;

	if (isc_service_attach(	status,
							0,
							svc_name,
							(&svc_handle),
							spblen, spb))
	{
		BURP_print_status(status);
		gds__free(spb);
		gds__free(svc_name);
		BURP_print(83, 0, 0, 0, 0, 0);	/* msg 83 Exiting before completion due to errors */
		return FINI_ERROR;
	}

	thd = (char *) gds__alloc((SLONG) (2));
	/* 'isc_action_svc_restore/isc_action_svc_backup'
	   'isc_spb_verbose' */

	if (thd == NULL) {
		status[0] = isc_arg_gds;
		status[1] = isc_virmemexh;
		status[2] = isc_arg_end;
		BURP_print_status(status);
		isc_service_detach(status, (&svc_handle));
		gds__free(spb);
		gds__free(svc_name);
		BURP_print(83, 0, 0, 0, 0, 0);	/* msg 83 Exiting before completion due to errors */
		return FINI_ERROR;
	}

	thd_ptr = thd;
	if (restore)
		*thd_ptr++ = isc_action_svc_restore;
	else
		*thd_ptr++ = isc_action_svc_backup;

	if (verbose)
		*thd_ptr++ = isc_spb_verbose;

	thdlen = thd_ptr - thd;

	if (isc_service_start(	status,
							(&svc_handle),
							NULL,
							thdlen,
							thd))
	{
		BURP_print_status(status);
		gds__free(spb);
		gds__free(svc_name);
		gds__free(thd);
		isc_service_detach(status, (&svc_handle));
		BURP_print(83, 0, 0, 0, 0, 0);	/* msg 83 Exiting before completion due to errors */
		return FINI_ERROR;
	}

	do {
		if (isc_service_query(	status,
								(&svc_handle),
								NULL,
								0,
								NULL,
								sizeof(sendbuf),
								sendbuf,
								sizeof(respbuf),
								respbuf))
		{
			BURP_print_status(status);
			gds__free(spb);
			gds__free(svc_name);
			gds__free(thd);
			isc_service_detach(status,
							   (&svc_handle));
			BURP_print(83, 0, 0, 0, 0, 0);	/* msg 83 Exiting before completion due to errors */
			return FINI_ERROR;
		}

		x = p = respbuf;

		if (*p++ == isc_info_svc_line)
		{
			ISC_USHORT len = 0;

			len = (ISC_USHORT) isc_vax_integer(p, sizeof(ISC_USHORT));
			p += sizeof(ISC_USHORT);
			if (!len)
			{
				if (*p == isc_info_data_not_ready)
					continue;
				else if (*p == isc_info_end)
					break;
			}

			p[len] = '\0';
			burp_output("%s\n", p);

			p += len;
		}
	}
	while (*x == isc_info_svc_line);

	isc_service_detach(status, (&svc_handle));
	return FINI_OK;
}

#endif	// !SUPERSERVER

