/*
 *	PROGRAM:	JRD Backup and Restore Program
 *	MODULE:		burp.cpp
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
 * 2002.10.30 Sean Leyne - Removed support for obsolete "PC_PLATFORM" define
 */

#include "firebird.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "../jrd/common.h"
#include "../jrd/ibase.h"
#include <stdarg.h>
#include "../jrd/ibsetjmp.h"
#include "../jrd/msg_encode.h"
#include "../jrd/thd.h"
#include "../jrd/ods.h"			// to get MAX_PAGE_SIZE
#include "../jrd/svc.h"
#include "../burp/burp.h"
#include "../burp/burpswi.h"
#include "../burp/std_desc.h"
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
#include "../utilities/common/cmd_util_proto.h"
#endif

#ifdef UNIX
#include <unistd.h>
#endif

#ifdef VMS
#include <descrip.h>
#include <iodef.h>
#include <types.h>
#include <file.h>
#endif

#ifndef VMS
#include <fcntl.h>
#endif

#ifdef WIN_NT
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

const char* fopen_write_type = "w";
const char* fopen_read_type	 = "r";

const int open_mask	 = 0666;

#ifdef VMS
const char* switch_char	= "/";
#else
const char* switch_char	= "-";
#endif


const char* const output_suppress	= "SUPPRESS";
const int burp_msg_fac				= 12;

enum gbak_action
{
	QUIT	=	0,
	BACKUP	=	1 ,
	RESTORE	=	2,
	FDESC	=	3
};

static void close_out_transaction(gbak_action, isc_tr_handle*);
//static void enable_signals(void);
//static void excp_handler(void);
static SLONG get_number(const SCHAR*);
static ULONG get_size(const SCHAR*, burp_fil*);
static gbak_action open_files(const TEXT *, const TEXT**, bool, USHORT);
static int common_main(int, char**, Jrd::pfn_svc_output, Jrd::Service*);
#ifndef SUPERSERVER
BurpGlobals* gdgbl;
static int output_main(Jrd::Service*, const UCHAR*);
static int api_gbak(int, char**, USHORT, TEXT*, TEXT*, TEXT *, bool, bool);
#endif
static void burp_output(const SCHAR*, ...) ATTRIBUTE_FORMAT(1,2);

// fil.fil_length is ULONG
const ULONG KBYTE	= 1024;
const ULONG MBYTE	= KBYTE * KBYTE;
const ULONG GBYTE	= MBYTE * KBYTE;

#if defined (WIN95)
static bool fAnsiCP = false;
static inline void translate_cp(SCHAR* a)
{
	if (!fAnsiCP) 
		AnsiToOem(a, a);
}
#else
static inline void translate_cp(SCHAR* a)
{
}
#endif

static int output_svc(Jrd::Service* output_data, const UCHAR* output_buf)
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
	fprintf(stdout, "%s", output_buf);
	return 0;
}

#ifdef SUPERSERVER
THREAD_ENTRY_DECLARE BURP_main(THREAD_ENTRY_PARAM arg)
{
/**************************************
 *
 *	m a i n _ g b a k
 *
 **************************************
 *
 * Functional description
 *	Entrypoint for GBAK via services manager.
 *
 **************************************/
	Jrd::Service* service = (Jrd::Service*)arg;
	const int exit_code = common_main(service->svc_argc, service->svc_argv,
						  SVC_output, service);

// Mark service thread as finished. 
// If service is detached, cleanup memory being used by service. 
	SVC_finish(service, Jrd::SVC_finished);

	return (THREAD_ENTRY_RETURN)(exit_code);
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
// If a "-service" switch is specified then use Admin APIs
// The code will alter the command line.
	TEXT** argvp = argv;
	const TEXT* const* const end = argvp + argc;
	argvp++;

// Initialize data 
	USHORT total = 0;
	bool flag_restore, flag_verbose, err;
	flag_restore = flag_verbose = err = false;
	
	TEXT *sw_user, *sw_password, *sw_service;
	TEXT *d_user, *d_password, *d_service;
	sw_user = sw_password = sw_service = d_user = d_password = d_service = NULL;

/* Parse the command line for the -USER, -PASSWORD, -SERVICE,
   and -VERBOSE options. Calculate a length for the new command line to be
   passed to a server using services APIs */
   
	// This is to avoid overwriting literal strings later. Read the warning near
	// the end of this function.
	char none[] = "-*NONE*";

	while (argvp < end && !err)
	{
		TEXT* string = *argvp++;
		if (*string != '-') {
			total += strlen(string) + 1;
			continue;
		}
		if (!string[1])
			string = none;
		const in_sw_tab_t* in_sw_tab = burp_in_sw_table;
		const TEXT* q;
		for (; q = in_sw_tab->in_sw_name; in_sw_tab++)
		{
			TEXT c;
			for (const TEXT* p = string + 1; c = *p++;) {
				if (UPPER(c) != *q++)
					break;
			}
			if (!c)
				break;
		}
		switch (in_sw_tab->in_sw)
		{
		case IN_SW_BURP_C:		// create database 
		case IN_SW_BURP_R:		// replace database 
			total += strlen(string) + 1;
			flag_restore = true;
			break;
		case IN_SW_BURP_USER:	// default user name 
			if (argvp >= end)
				err = true;
			else {
				sw_user = string;
				d_user = *argvp++;
			}
			break;
		case IN_SW_BURP_PASS:	// default password 
			if (argvp >= end)
				err = true;
			else {
				sw_password = string;
				d_password = *argvp++;
			}
			break;
		case IN_SW_BURP_SE:	// service name 
			if (argvp >= end) {
				err = true;
			}
			else {
				sw_service = string;
				d_service = *argvp++;
			}
			break;
		case IN_SW_BURP_V:		// verify actions 
			total += strlen(string) + 1;
			flag_verbose = true;
			break;
		default:
			total += strlen(string) + 1;
			break;
		}
	}

	int exit_code;
	if (sw_service && !err)
	{
		/* Backup/restore operations will be running as a service thread.
		 * To make this more efficiently the isc_spb_options is used.
		 * This allows us to skip a conversion from the gbak command line
		 * switches to service parameter block in here as well as vice versa
		 * conversion within svc.cpp
		 *
		 * If -USER and -PASSWORD switches are used by the user within
		 * the gbak command line then we have to eliminate them from there. The
		 * password will be encrypted and added along with the user name
		 * within SVC_start function later on. We shall also eliminate
		 * the -SERVER switch because the switch has already been processed.
		 */

		// Warning: altering command line.
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
		exit_code = common_main(argc, argv, output_main, NULL);

	return exit_code;
}


static int output_main(Jrd::Service* output_data, const UCHAR* output_buf)
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
	fprintf(stderr, "%s", output_buf);
	return 0;
}

static int api_gbak(int argc,
					char* argv[],
					USHORT length,
					TEXT* password,
					TEXT* user,
					TEXT* service,
					bool restore,
					bool verbose)
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
	BurpGlobals ldgbl;
	BurpGlobals* tdgbl = &ldgbl;
	BurpGlobals::putSpecific(tdgbl);
	tdgbl->output_proc = output_main;

    const TEXT* usr;
	if (!user)
		usr = getenv("ISC_USER");
	else
		usr = user;

	const TEXT* pswd;
	if (!password)
		pswd = getenv("ISC_PASSWORD");
	else
		pswd = password;

	char *const spb = (char *) gds__alloc((SLONG) (2 + 2 + ((usr) ? strlen(usr) : 0) +
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
	ISC_STATUS_ARRAY status;
	
	if (spb == NULL) {
		status[0] = isc_arg_gds;
		status[1] = isc_virmemexh;
		status[2] = isc_arg_end;
		BURP_print_status(status);
		BURP_print(83, 0, 0, 0, 0, 0);	// msg 83 Exiting before completion due to errors 
		return FINI_ERROR;
	}

	char* spb_ptr = spb;
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

	char* const svc_name = (char *) gds__alloc((SLONG) (strlen(service) + 1));

	if (svc_name == NULL) {
		status[0] = isc_arg_gds;
		status[1] = isc_virmemexh;
		status[2] = isc_arg_end;
		BURP_print_status(status);
		gds__free(spb);
		BURP_print(83, 0, 0, 0, 0, 0);	// msg 83 Exiting before completion due to errors 
		return FINI_ERROR;
	}

	if (service) {
		strcpy(svc_name, service);
		*service = '\0';
	}


// Fill command line options 

	*spb_ptr++ = isc_spb_command_line;
	const TEXT* const* const begin = argv;
	const TEXT* const* const end = argv + argc;
	argv++;

	*spb_ptr++ = length;
	while (argv < end) {
		if (**argv && argv > begin + 1)
			*spb_ptr++ = ' ';
		for (const TEXT* x = *argv++; *x;)
			*spb_ptr++ = *x++;
	}

	USHORT spblen = spb_ptr - spb;

	FB_API_HANDLE svc_handle = 0;
	if (isc_service_attach(status, 0, svc_name, (&svc_handle), spblen, spb))
	{
		BURP_print_status(status);
		gds__free(spb);
		gds__free(svc_name);
		BURP_print(83, 0, 0, 0, 0, 0);
		// msg 83 Exiting before completion due to errors 
		return FINI_ERROR;
	}

	char *const thd = (char *) gds__alloc((SLONG) (2));
	// 'isc_action_svc_restore/isc_action_svc_backup'
	// 'isc_spb_verbose'

	if (thd == NULL) {
		status[0] = isc_arg_gds;
		status[1] = isc_virmemexh;
		status[2] = isc_arg_end;
		BURP_print_status(status);
		isc_service_detach(status, (&svc_handle));
		gds__free(spb);
		gds__free(svc_name);
		BURP_print(83, 0, 0, 0, 0, 0);
		// msg 83 Exiting before completion due to errors 
		return FINI_ERROR;
	}

	char *thd_ptr = thd;
	if (restore)
		*thd_ptr++ = isc_action_svc_restore;
	else
		*thd_ptr++ = isc_action_svc_backup;

	if (verbose)
		*thd_ptr++ = isc_spb_verbose;

	USHORT thdlen = thd_ptr - thd;

	if (isc_service_start(status, (&svc_handle), NULL, thdlen, thd))
	{
		BURP_print_status(status);
		gds__free(spb);
		gds__free(svc_name);
		gds__free(thd);
		isc_service_detach(status, (&svc_handle));
		BURP_print(83, 0, 0, 0, 0, 0);	// msg 83 Exiting before completion due to errors 
		return FINI_ERROR;
	}

    const char sendbuf[] = { isc_info_svc_line };
	char respbuf[1024];
	const char* sl;
	do {
		if (isc_service_query(status, (&svc_handle), NULL, 0, NULL,
								sizeof(sendbuf), sendbuf,
								sizeof(respbuf), respbuf))
		{
			BURP_print_status(status);
			gds__free(spb);
			gds__free(svc_name);
			gds__free(thd);
			isc_service_detach(status, (&svc_handle));
			BURP_print(83, 0, 0, 0, 0, 0);	// msg 83 Exiting before completion due to errors 
			return FINI_ERROR;
		}

		char* p = respbuf;
		sl = p;

		if (*p++ == isc_info_svc_line)
		{
			const ISC_USHORT len = (ISC_USHORT) isc_vax_integer(p, sizeof(ISC_USHORT));
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
		}
	} while (*sl == isc_info_svc_line);

	gds__free(spb);
	gds__free(svc_name);
	gds__free(thd);
	isc_service_detach(status, (&svc_handle));
	return FINI_OK;
}


#endif	// SUPERSERVER


int common_main(int		argc,
				char*		argv[],
				Jrd::pfn_svc_output output_proc,
				Jrd::Service*		output_data)
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
	const TEXT* file2 = NULL;

  	JMP_BUF					env;

// TMN: This variable should probably be removed, but I left it in 
// in case some platform should redefine the BURP BurpGlobals::putSpecific. 
//BurpGlobals	thd_context;

	gbak_action action = QUIT;
	BurpGlobals sgbl;
	BurpGlobals *tdgbl = &sgbl;

	BurpGlobals::putSpecific(tdgbl);
	tdgbl->burp_env = reinterpret_cast<UCHAR*>(env);
	tdgbl->file_desc = INVALID_HANDLE_VALUE;
	tdgbl->output_proc = output_proc;
	tdgbl->output_data = output_data;

	in_sw_tab_t* in_sw_tab; // used in several parts below.
    
// Initialize static data. 
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
	tdgbl->gbl_sw_service_gbak = false;
	tdgbl->gbl_sw_service_thd = false;
	tdgbl->service_blk = NULL;
	tdgbl->status = tdgbl->status_vector;

	if (argc > 1 && !strcmp(argv[1], "-svc")) {
		tdgbl->gbl_sw_service_gbak = true;
		argv++;
		argc--;
	}
	else if (argc > 1 && !strcmp(argv[1], "-svc_thd")) {
		tdgbl->gbl_sw_service_gbak = true;
		tdgbl->gbl_sw_service_thd = true;
		tdgbl->service_blk = (Jrd::Service*) output_data;
		tdgbl->status = tdgbl->service_blk->svc_status;
		argv++;
		argc--;
	}
	else if (argc > 4 && !strcmp(argv[1], "-svc_re")) {
		tdgbl->gbl_sw_service_gbak = true;
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


	USHORT sw_replace = FALSE;

	tdgbl->gbl_sw_compress = true;
	tdgbl->gbl_sw_convert_ext_tables = false;
	tdgbl->gbl_sw_transportable = true;
	tdgbl->gbl_sw_ignore_limbo = false;
	tdgbl->gbl_sw_blk_factor = 0;
	tdgbl->gbl_sw_no_reserve = false;
	tdgbl->gbl_sw_old_descriptions = false;
	tdgbl->gbl_sw_mode = false;
	tdgbl->gbl_sw_skip_count = 0;
	tdgbl->action = NULL;

	tdgbl->dpb_length = 0;
	burp_fil* file = NULL;
	burp_fil* file_list = NULL;
	tdgbl->io_buffer_size = GBAK_IO_BUFFER_SIZE;
	
	// Avoid overwriting literal strings.
	char none[] = "-*NONE*";
	
	const TEXT* const* const end = argv + argc;
	++argv;

	while (argv < end) {
		TEXT* string = *argv;
		int temp = strlen(string) - 1;
		if (string[temp] == ',')
			string[temp] = '\0'; // Modifying argv elements

		if (*string != '-') {
			if (!file || file->fil_length || !get_size(*argv, file)) {
				/*  Miserable thing must be a filename
				   (dummy in a length for the backup file */

				file = (burp_fil*) BURP_alloc_zero(FIL_LEN);
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
				string = none;
			const TEXT* q;
			for (in_sw_tab = burp_in_sw_table; q = in_sw_tab->in_sw_name;
				 in_sw_tab++)
			{
			    TEXT c;
				for (const TEXT *p = string + 1; c = *p++;)
					if (UPPER(c) != *q++)
						break;
				if (!c)
					break;
			}
			in_sw_tab->in_sw_state = TRUE;
			if (!in_sw_tab->in_sw) {
				BURP_print(137, string + 1, 0, 0, 0, 0);
				// msg 137  unknown switch %s 
				BURP_print(95, 0, 0, 0, 0, 0);
				// msg 95  legal switches are
				for (in_sw_tab = burp_in_sw_table; in_sw_tab->in_sw;
					 in_sw_tab++)
					if (in_sw_tab->in_sw_msg) {
						BURP_msg_put(in_sw_tab->in_sw_msg, (void*)switch_char, 0, 0,
									 0, 0);
					}

				BURP_print(132, 0, 0, 0, 0, 0);
				// msg 132 switches can be abbreviated to one character 
				BURP_error(1, true, 0, 0, 0, 0, 0);
				// msg 1: found unknown switch 
			}
			else if (in_sw_tab->in_sw == IN_SW_BURP_S) {
				if (argv >= end)
					BURP_error(200, true, 0, 0, 0, 0, 0);
				// msg 200: missing parameter for the number of bytes to be skipped 
				tdgbl->gbl_sw_skip_count = get_number(*argv);
				if (!tdgbl->gbl_sw_skip_count)
					BURP_error(201, true, *argv, 0, 0, 0, 0);
				// msg 201: expected number of bytes to be skipped, encountered "%s" 
				argv++;
			}
			else if (in_sw_tab->in_sw == IN_SW_BURP_P) {
				if (argv >= end)
					BURP_error(2, true, 0, 0, 0, 0, 0);
					// msg 2 page size parameter missing 
				tdgbl->gbl_sw_page_size = (USHORT) get_number(*argv);
				if (!tdgbl->gbl_sw_page_size)
					BURP_error(12, true, *argv, 0, 0, 0, 0);
					// msg 12 expected page size, encountered "%s" 
				argv++;
			}
			else if (in_sw_tab->in_sw == IN_SW_BURP_BU) {
				if (argv >= end)
					BURP_error(258, true, 0, 0, 0, 0, 0);
					// msg 258 page buffers parameter missing 
				tdgbl->gbl_sw_page_buffers = get_number(*argv);
				if (!tdgbl->gbl_sw_page_buffers)
					BURP_error(259, true, *argv, 0, 0, 0, 0);
					// msg 259 expected page buffers, encountered "%s" 
				argv++;
			}
			else if (in_sw_tab->in_sw == IN_SW_BURP_MODE) {
				if (argv >= end)
					BURP_error(279, true, 0, 0, 0, 0, 0);
					// msg 279: "read_only" or "read_write" required 
				string = *argv++;
				if (!strcmp(string, BURP_SW_MODE_RO))
					tdgbl->gbl_sw_mode_val = true;
				else if (!strcmp(string, BURP_SW_MODE_RW))
					tdgbl->gbl_sw_mode_val = false;
				else
					BURP_error(279, true, 0, 0, 0, 0, 0);
					// msg 279: "read_only" or "read_write" required 
				tdgbl->gbl_sw_mode = true;
			}
			else if (in_sw_tab->in_sw == IN_SW_BURP_PASS) {
				if (argv >= end)
					BURP_error(189, true, 0, 0, 0, 0, 0);
					// password parameter missing 
				tdgbl->gbl_sw_password = *argv++;
			}
			else if (in_sw_tab->in_sw == IN_SW_BURP_USER) {
				if (argv >= end)
					BURP_error(188, true, 0, 0, 0, 0, 0);
					// user name parameter missing 
				tdgbl->gbl_sw_user = *argv++;
			}
			else if (in_sw_tab->in_sw == IN_SW_BURP_ROLE) {
				if (argv >= end)
					BURP_error(253, true, 0, 0, 0, 0, 0);
					// SQL role parameter missing 
				tdgbl->gbl_sw_sql_role = *argv++;
			}
			else if (in_sw_tab->in_sw == IN_SW_BURP_FA) {
				if (argv >= end)
					BURP_error(182, true, 0, 0, 0, 0, 0);
					// msg 182 blocking factor parameter missing 
				tdgbl->gbl_sw_blk_factor = get_number(*argv);
				if (!tdgbl->gbl_sw_blk_factor)
					BURP_error(183, true, *argv, 0, 0, 0, 0);	
					// msg 183 expected blocking factor, encountered "%s"  
				argv++;
			}
			else if (in_sw_tab->in_sw == IN_SW_BURP_SE) {
				if (argv >= end) {
					BURP_error(273, true, 0, 0, 0, 0, 0);
					// msg 273: service name parameter missing 
				}
				in_sw_tab->in_sw_state = FALSE;
				++argv;			// skip a service specification 
			}
			// want to do output redirect handling now instead of waiting
			else if (in_sw_tab->in_sw == IN_SW_BURP_Y) {
				const TEXT* redirect = *argv;
				if (argv >= end)	// redirect may equal NULL 
					redirect = NULL;
				else if (*redirect == '-')
					redirect = NULL;
				else
					++argv;
				if (!redirect)
					BURP_error(4, true, 0, 0, 0, 0, 0);
				// msg 4 redirect location for output is not specified 

				const TEXT *p = redirect;
				TEXT c;
				const TEXT* q2 = output_suppress;
				tdgbl->sw_redirect = NOOUTPUT;
				while (c = *p++) {
					if (UPPER(c) != *q2++) {
						tdgbl->sw_redirect = REDIRECT;
						break;
					}
				}
				if (tdgbl->sw_redirect == REDIRECT) { // not NOREDIRECT, and not NOOUTPUT 

					// Make sure the status file doesn't already exist 
					FILE* tmp_outfile = fopen(redirect, fopen_read_type);
					if (tmp_outfile) {
						BURP_print(66, redirect, 0, 0, 0, 0);
						// msg 66 can't open status and error output file %s 
						fclose(tmp_outfile);
						BURP_exit_local(FINI_ERROR, tdgbl);
					}
					if (!
						(tdgbl->output_file =
						 fopen(redirect, fopen_write_type)))
					{
						BURP_print(66, redirect, 0, 0, 0, 0);
						// msg 66 can't open status and error output file %s 
						BURP_exit_local(FINI_ERROR, tdgbl);
					}
				}
			}					//else if (in_sw_tab->in_sw == IN_SW_BURP_Y) 
		}						// else 
	}							// while (argv < end) 

// reverse the linked list of file blocks 

	tdgbl->gbl_sw_files = NULL;

    burp_fil* next_file = NULL;
	for (file = file_list; file; file = next_file) {
		next_file = file->fil_next;
		file->fil_next = tdgbl->gbl_sw_files;
		tdgbl->gbl_sw_files = file;
	}

// pop off the obviously boring ones, plus do some checking 

	const TEXT* file1 = NULL;
	for (file = tdgbl->gbl_sw_files; file; file = file->fil_next) {
		if (!file1)
			file1 = file->fil_name;
		else if (!file2)
			file2 = file->fil_name;
		for (file_list = file->fil_next; file_list;
			 file_list = file_list->fil_next)
		{
			if (!strcmp(file->fil_name, file_list->fil_name))
				BURP_error(9, true, 0, 0, 0, 0, 0);
			// msg 9 mutiple sources or destinations specified 
		}

	}

	// Initialize 'dpb' and 'dpb_length' and cast away volatile
	UCHAR* dpb = tdgbl->dpb_string;
	*dpb++ = isc_dpb_version1;
	*dpb++ = isc_dpb_gbak_attach;
	*dpb++ = strlen(GDS_VERSION);
	for (const TEXT* gvp = GDS_VERSION; *gvp;)
		*dpb++ = *gvp++;
	tdgbl->dpb_length = dpb - tdgbl->dpb_string;

	for (in_sw_tab = burp_in_sw_table; in_sw_tab->in_sw_name; in_sw_tab++) {
		if (in_sw_tab->in_sw_state) {
			switch (in_sw_tab->in_sw) {
			case (IN_SW_BURP_B):
				if (sw_replace)
					BURP_error(5, true, 0, 0, 0, 0, 0);
				// msg 5 conflicting switches for backup/restore 
				sw_replace = IN_SW_BURP_B;
				break;

			case (IN_SW_BURP_C):
				if (sw_replace == IN_SW_BURP_B)
					BURP_error(5, true, 0, 0, 0, 0, 0);
				// msg 5 conflicting switches for backup/restore 
				if (sw_replace != IN_SW_BURP_R)
					sw_replace = IN_SW_BURP_C;
				break;

			case (IN_SW_BURP_CO):
				tdgbl->gbl_sw_convert_ext_tables = true;
				break;

			case (IN_SW_BURP_E):
				tdgbl->gbl_sw_compress = false;
				break;

			case (IN_SW_BURP_G):
				if (!tdgbl->dpb_length)
					*dpb++ = isc_dpb_version1;
				*dpb++ = isc_dpb_no_garbage_collect;
				*dpb++ = 0;
				tdgbl->dpb_length = dpb - tdgbl->dpb_string;
				break;

			case (IN_SW_BURP_I):
				tdgbl->gbl_sw_deactivate_indexes = true;
				break;

			case (IN_SW_BURP_IG):
				if (!tdgbl->dpb_length)
					*dpb++ = isc_dpb_version1;
				*dpb++ = isc_dpb_damaged;
				*dpb++ = 1;
				*dpb++ = 1;
				tdgbl->dpb_length = dpb - tdgbl->dpb_string;
				break;

			case (IN_SW_BURP_K):
				tdgbl->gbl_sw_kill = true;
				break;

			case (IN_SW_BURP_L):
				tdgbl->gbl_sw_ignore_limbo = true;
				break;

			case (IN_SW_BURP_M):
				tdgbl->gbl_sw_meta = true;
				break;

			case (IN_SW_BURP_MODE):
				tdgbl->gbl_sw_mode = true;
				break;

			case (IN_SW_BURP_N):
				tdgbl->gbl_sw_novalidity = true;
				break;

			case (IN_SW_BURP_NT):	// Backup non-transportable format 
				tdgbl->gbl_sw_transportable = false;
				break;

			case (IN_SW_BURP_O):
				tdgbl->gbl_sw_incremental = true;
				break;

			case (IN_SW_BURP_OL):
				tdgbl->gbl_sw_old_descriptions = true;
				break;

			case (IN_SW_BURP_PASS):
				{
					if (!tdgbl->dpb_length)
						*dpb++ = isc_dpb_version1;
					if (!tdgbl->gbl_sw_service_gbak)
						*dpb++ = isc_dpb_password;
					else
						*dpb++ = isc_dpb_password_enc;
					*dpb++ = strlen(tdgbl->gbl_sw_password);
					for (const TEXT* q = tdgbl->gbl_sw_password; *q;)
						*dpb++ = *q++;
					tdgbl->dpb_length = dpb - tdgbl->dpb_string;
					break;
				}

			case (IN_SW_BURP_R):
				if (sw_replace == IN_SW_BURP_B)
					BURP_error(5, true, 0, 0, 0, 0, 0);
				// msg 5 conflicting switches for backup/restore 
				sw_replace = IN_SW_BURP_R;
				break;

			case (IN_SW_BURP_T):
				tdgbl->gbl_sw_transportable = true;
				break;

			case (IN_SW_BURP_U):
				BURP_error(7, true, 0, 0, 0, 0, 0);
				// msg 7 protection isn't there yet 
				break;

			case (IN_SW_BURP_US):
				tdgbl->gbl_sw_no_reserve = true;
				break;

			case (IN_SW_BURP_ROLE):
				{
					if (!tdgbl->dpb_length)
						*dpb++ = isc_dpb_version1;
					*dpb++ = isc_dpb_sql_role_name;
					*dpb++ = strlen(tdgbl->gbl_sw_sql_role);
					for (const TEXT* q = tdgbl->gbl_sw_sql_role; *q;)
						*dpb++ = *q++;
					tdgbl->dpb_length = dpb - tdgbl->dpb_string;
					break;
				}

			case (IN_SW_BURP_USER):
				{
					if (!tdgbl->dpb_length)
						*dpb++ = isc_dpb_version1;
					*dpb++ = isc_dpb_user_name;
					*dpb++ = strlen(tdgbl->gbl_sw_user);
					for (const TEXT* q = tdgbl->gbl_sw_user; *q;)
						*dpb++ = *q++;
					tdgbl->dpb_length = dpb - tdgbl->dpb_string;
					break;
				}

			case (IN_SW_BURP_V):
				tdgbl->gbl_sw_verbose = true;
				break;

			case (IN_SW_BURP_Z):
				BURP_print(91, (void*) GDS_VERSION, 0, 0, 0, 0);
				// msg 91 gbak version %s 
				tdgbl->gbl_sw_version = true;
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
			BURP_error(8, true, 0, 0, 0, 0, 0);
		// msg 8 page size is allowed only on restore or create 
		int temp = tdgbl->gbl_sw_page_size;
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
			BURP_error(3, true, isc_arg_number,
							(void*)(IPTR) tdgbl->gbl_sw_page_size,
							0, NULL, 0, NULL,
							0, NULL, 0, NULL);
			// msg 3 Page size specified (%ld) greater than limit (MAX_PAGE_SIZE bytes) 
		}
		if (temp != tdgbl->gbl_sw_page_size) {
			BURP_print(103, (void*)(IPTR) tdgbl->gbl_sw_page_size,
					   (void*)(IPTR) temp, 0, 0, 0);
			// msg 103 page size specified (%ld bytes) rounded up to %ld bytes 
			tdgbl->gbl_sw_page_size = temp;
		}
	}

	if (tdgbl->gbl_sw_page_buffers) {
		if (sw_replace == IN_SW_BURP_B)
			BURP_error(260, true, 0, 0, 0, 0, 0);
		// msg 260 page buffers is allowed only on restore or create 
	}

	if (!tdgbl->gbl_sw_blk_factor || sw_replace != IN_SW_BURP_B)
		tdgbl->gbl_sw_blk_factor = 1;

	if (!file2)
		BURP_error(10, true, 0, 0, 0, 0, 0);
	// msg 10 requires both input and output filenames 

	if (!strcmp(file1, file2))
		BURP_error(11, true, 0, 0, 0, 0, 0);
	// msg 11 input and output have the same name.  Disallowed. 

	time_t clock = time(NULL);
	strcpy(tdgbl->gbl_backup_start_time, ctime(&clock));
	TEXT* nlp = tdgbl->gbl_backup_start_time +
				strlen(tdgbl->gbl_backup_start_time) - 1;
	if (*nlp == '\n')
		*nlp = 0;

	tdgbl->action = (ACT) BURP_alloc_zero(ACT_LEN);
	tdgbl->action->act_total = 0;
	tdgbl->action->act_file = NULL;
	tdgbl->action->act_action = ACT_unknown;

	action =
		open_files(file1, &file2, tdgbl->gbl_sw_verbose, sw_replace);

	MVOL_init(tdgbl->io_buffer_size);
	
	int result;

	switch (action) {
	case (RESTORE):
		tdgbl->service_blk->svc_started();
		result = RESTORE_restore(file1, file2);
		break;

	case (BACKUP):
		tdgbl->service_blk->svc_started();
		result = BACKUP_backup(file1, file2);
		break;

	case (QUIT):
		tdgbl->service_blk->svc_started();
		BURP_abort();
		return 0;
	
	default:
		// result undefined
		fb_assert(false);
		return 0;
	}
	if (result != FINI_OK && result != FINI_DB_NOT_ONLINE)
		BURP_abort();

	BURP_exit_local(result, tdgbl);
	return result;
	}	// try

	catch (const std::exception&)
	{
		// All calls to exit_local(), normal and error exits, wind up here 

		tdgbl->burp_env = NULL;
		const int exit_code = tdgbl->exit_code;

		// Close the gbak file handles if they still open 
		for (burp_fil* file = tdgbl->gbl_sw_backup_files; file; file = file->fil_next)
		{
			if (file->fil_fd != INVALID_HANDLE_VALUE)
				close_platf(file->fil_fd);
			if (exit_code != 0
				&& (tdgbl->action->act_action == ACT_backup_split
					|| tdgbl->action->act_action == ACT_backup))
			{
				unlink_platf(file->fil_name);
			}
		}

		// Detach from database to release system resources 
		if (tdgbl->db_handle != 0) {
			close_out_transaction(action, &tdgbl->tr_handle);
			close_out_transaction(action, &tdgbl->global_trans);
			if (isc_detach_database(tdgbl->status_vector, &tdgbl->db_handle))
			{
				BURP_print_status(tdgbl->status_vector);
			}
		}

		// Close the status output file 
		if (tdgbl->sw_redirect == REDIRECT && tdgbl->output_file != NULL) {
			fclose(tdgbl->output_file);
			tdgbl->output_file = NULL;
		}

		// Free all unfreed memory used by Gbak itself 
		while (tdgbl->head_of_mem_list != NULL) {
			UCHAR* mem = tdgbl->head_of_mem_list;
			tdgbl->head_of_mem_list = *((UCHAR **) tdgbl->head_of_mem_list);
			gds__free(mem);
		}

		BurpGlobals::restoreSpecific();

#if defined(DEBUG_GDS_ALLOC) && !defined(SUPERSERVER)
		gds_alloc_report(0, __FILE__, __LINE__);
#endif

		// All returns occur from this point - even normal returns 
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
	BurpGlobals* tdgbl = BurpGlobals::getSpecific();

	BURP_print(83, 0, 0, 0, 0, 0);
	// msg 83 Exiting before completion due to errors 

	tdgbl->service_blk->svc_started();

	BURP_exit_local(FINI_ERROR, tdgbl);
}

void BURP_error(USHORT errcode, bool abort,
					USHORT arg1_t,
					const void *arg1,
					USHORT arg2_t,
					const void *arg2,
					USHORT arg3_t,
					const void *arg3,
					USHORT arg4_t,
					const void *arg4,
					USHORT arg5_t,
					const void *arg5)
{
/**************************************
 *
 *      B U R P _ e r r o r
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
#ifdef SUPERSERVER
	BurpGlobals* tdgbl = BurpGlobals::getSpecific();

	ISC_STATUS *status = tdgbl->service_blk->svc_status;

	CMD_UTIL_put_svc_status(status, burp_msg_fac, errcode,
							arg1_t, arg1, arg2_t, arg2, arg3_t, arg3,
							arg4_t, arg4, arg5_t, arg5);

	tdgbl->service_blk->svc_started();
#endif
	BURP_msg_partial(256, 0, 0, 0, 0, 0);	// msg 256: gbak: ERROR: 
	BURP_msg_put(errcode, arg1, arg2, arg3, arg4, arg5);
	if (abort)
		BURP_abort();
}


void BURP_error(USHORT errcode, bool abort,
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
	BURP_error(errcode, abort, isc_arg_string, arg1, isc_arg_string, arg2,
				isc_arg_string, arg3, isc_arg_string, arg4,
				isc_arg_string, arg5);
}


void BURP_error_redirect(const ISC_STATUS* status_vector,
							USHORT errcode,
							const void* arg1,
							const void* arg2)
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
	BURP_error(errcode, true, arg1, arg2, NULL, NULL, NULL);
}


// Raises an exception when the old SEH system would jump to another place.
void BURP_exit_local(int code, BurpGlobals* tdgbl)
{
	tdgbl->exit_code = code;
	if (tdgbl->burp_env != NULL)
		throw std::exception();
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

	gds__msg_format(NULL, burp_msg_fac, number, sizeof(buffer), buffer,
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

	gds__msg_format(NULL, burp_msg_fac, number, sizeof(buffer), buffer,
					static_cast<const char*>(arg1),
					static_cast<const char*>(arg2),
					static_cast<const char*>(arg3),
					static_cast<const char*>(arg4),
					static_cast<const char*>(arg5));
	translate_cp(buffer);
	burp_output("%s\n", buffer);
}


void BURP_msg_get(	USHORT number,
					TEXT* output_msg,
					const void* arg1,
					const void* arg2,
					const void* arg3,
					const void* arg4,
					const void* arg5)
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

	gds__msg_format(NULL, burp_msg_fac, number, sizeof(buffer), buffer,
					static_cast<const char*>(arg1),
					static_cast<const char*>(arg2),
					static_cast<const char*>(arg3),
					static_cast<const char*>(arg4),
					static_cast<const char*>(arg5));
	strcpy(output_msg, buffer);
}


void BURP_output_version(void* arg1, const TEXT* arg2)
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

	burp_output(static_cast<const char*>(arg1), arg2);
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

	BURP_msg_partial(169, 0, 0, 0, 0, 0);	// msg 169: gbak: 
	BURP_msg_put(number, arg1, arg2, arg3, arg4, arg5);
}


void BURP_print_status(const ISC_STATUS* status_vector)
{
/**************************************
 *
 *	B U R P _ p r i n t _ s t a t u s
 *
 **************************************
 *
 * Functional description
 *	Print error message. Use isc_interpret
 *	to allow redirecting output.
 *
 **************************************/
	if (status_vector) {
		const ISC_STATUS* vector = status_vector;
#ifdef SUPERSERVER
		BurpGlobals* tdgbl = BurpGlobals::getSpecific();
		ISC_STATUS* status = tdgbl->service_blk->svc_status;
		if (status != status_vector) {
		    int i = 0;
			while (*status && (++i < ISC_STATUS_LENGTH))
				status++;
			for (int j = 0; status_vector[j] && (i < ISC_STATUS_LENGTH); j++, i++)
				*status++ = status_vector[j];
		}
#endif

        SCHAR s[1024];
		if (isc_interpret(s, sizeof(s), &vector)) {
			translate_cp(s);
			BURP_msg_partial(256, 0, 0, 0, 0, 0); // msg 256: gbak: ERROR: 
			burp_output("%s\n", s);
			while (isc_interpret(s, sizeof(s), &vector)) {
				translate_cp(s);
				BURP_msg_partial(256, 0, 0, 0, 0, 0); // msg 256: gbak: ERROR:
				burp_output("    %s\n", s);
			}
		}
	}
}


void BURP_print_warning(const ISC_STATUS* status_vector)
{
/**************************************
 *
 *	B U R P _ p r i n t _ w a r n i n g
 *
 **************************************
 *
 * Functional description
 *	Print warning message. Use isc_interpret
 *	to allow redirecting output.
 *
 **************************************/
	if (status_vector) {
		// skip the error, assert that one does not exist 
		fb_assert(status_vector[0] == isc_arg_gds);
		fb_assert(status_vector[1] == 0);
		// print the warning message 
		const ISC_STATUS* vector = &status_vector[2];
		SCHAR s[1024];
		if (isc_interpret(s, sizeof(s), &vector)) {
			translate_cp(s);
			BURP_msg_partial(255, 0, 0, 0, 0, 0); // msg 255: gbak: WARNING: 
			burp_output("%s\n", s);
			while (isc_interpret(s, sizeof(s), &vector)) {
				translate_cp(s);
				BURP_msg_partial(255, 0, 0, 0, 0, 0); // msg 255: gbak: WARNING: 
				burp_output("    %s\n", s);
			}
		}
	}
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
	BurpGlobals* tdgbl = BurpGlobals::getSpecific();

	if (tdgbl->gbl_sw_verbose)
		BURP_print(number, arg1, arg2, arg3, arg4, arg5);
	else
		burp_output("%s", "");
}


static void close_out_transaction(gbak_action action,
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


static SLONG get_number( const SCHAR* string)
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
 *  CVC: where does it complain? It does return zero, nothing else.
 **************************************/
	SCHAR c;
	SLONG value = 0;

	for (const SCHAR* p = string; c = *p++;) {
		if (c < '0' || c > '9')
			return 0;
		value *= 10;
		value += c - '0';
	}

	return value;
}


static gbak_action open_files(const TEXT* file1,
							  const TEXT** file2,
							  bool sw_verbose,
							  USHORT sw_replace)
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
	BurpGlobals* tdgbl = BurpGlobals::getSpecific();
	ISC_STATUS* status_vector = tdgbl->status;

// try to attach the database using the first file_name 

	if (sw_replace != IN_SW_BURP_C && sw_replace != IN_SW_BURP_R)
		if (!(isc_attach_database(status_vector,
								  (SSHORT) 0,
								  file1,
								  &tdgbl->db_handle,
								  tdgbl->dpb_length,
								  reinterpret_cast<char*>(tdgbl->dpb_string))))
		{
			if (sw_replace != IN_SW_BURP_B) {
				// msg 13 REPLACE specified, but the first file %s is a database 
				BURP_error(13, true, file1, 0, 0, 0, 0);
				if (isc_detach_database(status_vector, &tdgbl->db_handle)) {
					BURP_print_status(status_vector);
				}
				return QUIT;
			}
			if (tdgbl->gbl_sw_version) {
				// msg 139 Version(s) for database "%s" 
				BURP_print(139, file1, 0, 0, 0, 0);
				isc_version(&tdgbl->db_handle, BURP_output_version, (void*) "\t%s\n");
			}
			if (sw_verbose)
				BURP_print(166, file1, 0, 0, 0, 0);
				// msg 166: readied database %s for backup 
		}
		else if (sw_replace == IN_SW_BURP_B ||
				 (status_vector[1] != isc_io_error
				  && status_vector[1] != isc_bad_db_format)) {
			BURP_print_status(status_vector);
			return QUIT;
		}

	burp_fil* fil = 0;
	if (sw_replace == IN_SW_BURP_B) {

		// Now it is safe to skip a db file 
		tdgbl->gbl_sw_backup_files = tdgbl->gbl_sw_files->fil_next;
		tdgbl->gbl_sw_files = tdgbl->gbl_sw_files->fil_next;
		fb_assert(strcmp(tdgbl->gbl_sw_files->fil_name, *file2) == 0);

		gbak_action flag = BACKUP;
		tdgbl->action->act_action = ACT_backup;
		for (fil = tdgbl->gbl_sw_files; fil; fil = fil->fil_next)
		{
			// adjust the file size first 
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
				BURP_error(262, true, fil->fil_name, 0, 0, 0, 0);
				// msg 262 size specification either missing or incorrect for file %s
				break;
			default:
				fb_assert(FALSE);
				break;
			}

			if ((fil->fil_seq = ++(tdgbl->action->act_total)) >= 2)
			{
				tdgbl->action->act_action = ACT_backup_split;
			}
			if (sw_verbose)
			{
				BURP_print(75, fil->fil_name, 0, 0, 0, 0);	// msg 75  creating file %s 
			}
			if (!strcmp(fil->fil_name, "stdout"))
			{
				if (tdgbl->action->act_total >= 2 || fil->fil_next)
				{
					BURP_error(266, true, 0, 0, 0, 0, 0);
					// msg 266 standard output is not supported when using split operation
					flag = QUIT;
					break;
				}
				/* We ignore SIGPIPE so that we can report an IO error when we
				 * try to write to the broken pipe.
				 */
#ifndef WIN_NT
				signal(SIGPIPE, SIG_IGN);
#endif
				fil->fil_fd = reinterpret_cast<DESC>(GBAK_STDOUT_DESC());
				break;
			}
			else
			{

#ifdef WIN_NT
				if ((fil->fil_fd = MVOL_open(fil->fil_name, MODE_WRITE,
											 CREATE_ALWAYS)) == INVALID_HANDLE_VALUE)
#else
				if ((fil->fil_fd = open(fil->fil_name, MODE_WRITE, open_mask)) == -1)
#endif // WIN_NT 

				{

					BURP_error(65, false, isc_arg_string, fil->fil_name,
								   0, NULL, 0, NULL, 0, NULL, 0, NULL);
					// msg 65 can't open backup file %s
					flag = QUIT;
					break;
				}
			}

			if (fil->fil_length == 0)
			{
				if (fil->fil_next)
				{
					BURP_error(262, true, fil->fil_name, 0, 0, 0, 0);
					// msg 262 size specification either missing or incorrect for file %s
					flag = QUIT;
					break;
				}
				else
				{
					fil->fil_length = MAX_LENGTH;
					// Write as much as possible to the last file
				}
			}
			if (fil->fil_length < MIN_SPLIT_SIZE)
			{
				BURP_error(271, true,
								isc_arg_number,
								reinterpret_cast<void*>(fil->fil_length),
								isc_arg_number,
								reinterpret_cast<void*>(MIN_SPLIT_SIZE),
								0, NULL, 0, NULL, 0, NULL);
				// msg file size given (%d) is less than minimum allowed (%d) 
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
		fil->fil_fd = reinterpret_cast<DESC>(GBAK_STDIN_DESC());
		tdgbl->file_desc = fil->fil_fd;
		tdgbl->gbl_sw_files = fil->fil_next;
	}
	else {
		// open first file 
#ifdef WIN_NT
		if ((fil->fil_fd = MVOL_open(fil->fil_name, MODE_READ, OPEN_EXISTING))
			== INVALID_HANDLE_VALUE)
#else
		if ((fil->fil_fd = open(fil->fil_name, MODE_READ)) ==
			INVALID_HANDLE_VALUE)
#endif
		{
			BURP_error(65, true, fil->fil_name, 0, 0, 0, 0);
			// msg 65 can't open backup file %s 
			return QUIT;
		}

		if (sw_verbose)
			BURP_print(100, fil->fil_name, 0, 0, 0, 0);
			// msg 100 opened file %s
		// read and check a header record 
		tdgbl->action->act_file = fil;
		int seq = 1;
		if (MVOL_split_hdr_read() == TRUE) {
			tdgbl->action->act_action = ACT_restore_join;
			// number of files to be join 
			int total = tdgbl->action->act_total;
			if (fil->fil_seq != seq || seq > total) {
				BURP_error(263, true, fil->fil_name, 0, 0, 0, 0);
				// msg 263 file %s out of sequence 
				return QUIT;
			}

			for (++seq, fil = fil->fil_next; seq <= total;
				 fil = fil->fil_next, seq++)
			{
				if (!fil) {
					BURP_error(264, true, 0, 0, 0, 0, 0);
					// msg 264 can't join -- one of the files missing 
					return QUIT;
				}
				if (!strcmp(fil->fil_name, "stdin")) {
					BURP_error(265, true, 0, 0, 0, 0, 0);
					// msg 265 standard input is not supported when using join operation 
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
					BURP_error(65, false, isc_arg_string, fil->fil_name,
								   0, NULL, 0, NULL, 0, NULL, 0, NULL);
					// msg 65 can't open backup file %s
					return QUIT;
				}

				if (sw_verbose)
					BURP_print(100, fil->fil_name, 0, 0, 0, 0);
					// msg 100 opened file %s
				if (MVOL_split_hdr_read() == TRUE) {
					if ((total != tdgbl->action->act_total) ||
						(seq != fil->fil_seq) || (seq > total))
					{
						BURP_error(263, true, fil->fil_name, 0, 0, 0, 0);
						// msg 263 file %s out of sequence 
						return QUIT;
					}
				}
				else {
					BURP_error(267, true, fil->fil_name, 0, 0, 0, 0);
					// msg 267 backup file %s might be corrupt 
					return QUIT;
				}
			}
			tdgbl->action->act_file = tdgbl->gbl_sw_files;
			tdgbl->file_desc = tdgbl->action->act_file->fil_fd;
			if ((tdgbl->gbl_sw_files = fil) == NULL) {
				BURP_error(268, true, 0, 0, 0, 0, 0);
				// msg 268 database file specification missing 
				return QUIT;
			}
		}
		else {
			// Move pointer to the begining of the file. At this point we
			// assume -- this is a single backup file because we were
			// not able to read a split header record.
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


// If we got here, we've opened a backup file, and we're
// thinking about creating or replacing a database.

	*file2 = tdgbl->gbl_sw_files->fil_name;
	if (tdgbl->gbl_sw_files->fil_size_code != size_n)
		BURP_error(262, true, *file2, 0, 0, 0, 0);
	// msg 262 size specificati on either missing or incorrect for file %s  

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
			BURP_error(14, true, *file2, 0, 0, 0, 0);
			// msg 14 database %s already exists.  To replace it, use the -R switch 
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

				if (status_vector[1] != isc_unavailable)
					BURP_error(233, true, *file2, 0, 0, 0, 0);
				// msg 233 Cannot drop database %s, might be in use 
			}
		}
	}
	if (sw_replace == IN_SW_BURP_R && status_vector[1] == isc_adm_task_denied) {
		/* if we got an error from attach database and we have replace switch set
		 * then look for error from attach returned due to not owner, if we are
		 * not owner then return the error status back up
		 */
		BURP_error(274, true, 0, 0, 0, 0, 0);
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

// check the file size specification 
	for (fil = tdgbl->gbl_sw_files; fil; fil = fil->fil_next) {
		if (fil->fil_size_code != size_n)
			BURP_error(262, true, fil->fil_name, 0, 0, 0, 0);
			// msg 262 size specification either missing or incorrect for file %s  
	}

	return RESTORE;
}


static void burp_output( const SCHAR* format, ...)
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

	BurpGlobals* tdgbl = BurpGlobals::getSpecific();

	if (tdgbl->sw_redirect == NOOUTPUT || format[0] == '\0') {
		exit_code =
			tdgbl->output_proc(tdgbl->output_data,
							   (UCHAR*)(""));
	}
	else if (tdgbl->sw_redirect == REDIRECT && tdgbl->output_file != NULL) {
		va_start(arglist, format);
		vfprintf(tdgbl->output_file, format, arglist);
		va_end(arglist);
		exit_code =
			tdgbl->output_proc(tdgbl->output_data,
							   (UCHAR*)(""));
	}
	else {
		va_start(arglist, format);
		vsprintf((char *) buf, format, arglist);
		va_end(arglist);

		exit_code = tdgbl->output_proc(tdgbl->output_data, buf);
	}

	if (exit_code != 0)
		BURP_exit_local(exit_code, tdgbl);
}


static ULONG get_size( const SCHAR* string, burp_fil* file)
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
	SCHAR c;
	ULONG size = 0;
	bool digit = false;

	file->fil_size_code = size_n;
	for (const SCHAR *num = string; c = *num++;) {
		if (isdigit(c)) {
			size = size * 10 + (c - '0');
			digit = true;
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

	file->fil_length = size;
	return (size);
}

