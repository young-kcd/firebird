/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		svc.cpp
 *	DESCRIPTION:	Service manager functions
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
 * 2002.02.15 Sean Leyne - Code Cleanup, removed obsolete "IMP" port
 *
 * 2002.10.27 Sean Leyne - Completed removal of obsolete "DELTA" port
 * 2002.10.27 Sean Leyne - Completed removal of obsolete "IMP" port
 *
 * 2002.10.29 Sean Leyne - Removed obsolete "Netware" port
 *
 */

#include "firebird.h"
#include <stdio.h>
#include <string.h>
#include "../jrd/jrd_time.h"
#include "../jrd/common.h"
#include "../jrd/thd.h"
#include "../jrd/file_params.h"
#include <stdarg.h>
#include "../jrd/jrd.h"
#include "../jrd/svc.h"
#include "../jrd/jrd_pwd.h"
#include "../alice/aliceswi.h"
#include "../burp/burpswi.h"
#include "../jrd/ibase.h"
#include "gen/iberror.h"
#include "../jrd/license.h"
#include "../jrd/err_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/inf_proto.h"
#include "../jrd/isc_proto.h"
#include "../jrd/jrd_proto.h"
#include "../jrd/mov_proto.h"
#include "../jrd/sch_proto.h"
#include "../jrd/svc_proto.h"
#include "../jrd/thread_proto.h"
#include "../jrd/why_proto.h"
#include "../jrd/utl_proto.h"
#include "../jrd/jrd_proto.h"
#include "../jrd/enc_proto.h"
#include "../utilities/gsec/gsecswi.h"
#include "../utilities/gstat/dbaswi.h"
#include "../common/classes/alloc.h"
#ifdef SERVER_SHUTDOWN
#include "../jrd/jrd_proto.h"
#endif

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#ifdef HAVE_SYS_WAIT_H
# include <sys/wait.h>
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef HAVE_VFORK_H
#include <vfork.h>
#endif

#ifdef SOLARIS
#include <fcntl.h>
#endif

#ifdef SCO_UNIX
#include <fcntl.h>
#endif

const char* SYSDBA_USER_NAME	= "SYSDBA";
const int SVC_user_dba			= 2;
const int SVC_user_any			= 1;
const int SVC_user_none			= 0;

#if !defined(WIN_NT)
#  include <signal.h>
#  ifndef VMS
#    include <sys/param.h>
#    include <sys/stat.h>
#  else
#    include <stat.h>
#  endif
#  include <errno.h>
#else
#  include <windows.h>
#  include <io.h> // _open, _get_osfhandle
#  include <stdlib.h>
#  include <fcntl.h>
#  include <sys/stat.h>
#endif

#ifdef VMS
#define waitpid(x, y, z)	wait (y)
#endif

#define statistics	stat

const int GET_LINE		= 1;
const int GET_EOF		= 2;
const int GET_BINARY	= 4;

const TEXT SVC_TRMNTR	= '\377';

using namespace Jrd;

/* This checks if the service has forked a process.  If not,
   it will post the isc_svcnoexe error. */

static inline void is_service_running(const Service* service)
{
	if (!(service->svc_flags & SVC_forked)) {
		THREAD_ENTER();
		ERR_post (isc_svcnoexe, isc_arg_string,
			service->svc_service->serv_name, 0); 
	}
}

static inline void need_admin_privs(ISC_STATUS** status, const char* message)
{
	ISC_STATUS* stat = *status;
	*stat++ = isc_insufficient_svc_privileges;
	*stat++ = isc_arg_string;
	*stat++ = (ISC_STATUS)(U_IPTR) ERR_string(message, strlen(message));
	*stat++ = isc_arg_end;
	*status = stat;
}

bool ck_space_for_numeric(char*& info, const char* const end)
{
	if ((info + 1 + sizeof (ULONG)) > end)
    {
		if (info < end)
			*info++ = isc_info_truncated;
		THREAD_ENTER();
		return false;
	}
	return true;
}


/* Option block for service parameter block */
// What's the point of defining spb if a char* var will have the same name, Borland???
// It was easier to rename this struct.

struct Serv_param_block {
	TEXT*	spb_sys_user_name;
	TEXT*	spb_user_name;
	TEXT*	spb_password;
	TEXT*	spb_password_enc;
	TEXT*	spb_command_line;
	USHORT	spb_version;
};

static void conv_switches(USHORT, USHORT, const SCHAR*, TEXT**);
static const TEXT* find_switch(int, const in_sw_tab_t*);
static USHORT process_switches(USHORT, const SCHAR*, TEXT*);
static void get_options(const UCHAR*, USHORT, TEXT*, Serv_param_block*);
static TEXT* get_string_parameter(const UCHAR**, TEXT**);
#ifndef SUPERSERVER
static void io_error(const TEXT*, SLONG, const TEXT*, ISC_STATUS, bool);
static void service_close(Service*);
#endif
static bool get_action_svc_bitmask(const TEXT**, const in_sw_tab_t*,
									TEXT**, USHORT*, USHORT*);
static void get_action_svc_string(const TEXT**, TEXT**, USHORT*, USHORT*);
static void get_action_svc_data(const TEXT**, TEXT**, USHORT*, USHORT*);
static bool get_action_svc_parameter(const TEXT**, const in_sw_tab_t*,
									TEXT**, USHORT*, USHORT*);

#ifdef SUPERSERVER
static UCHAR service_dequeue_byte(Service*);
static void service_enqueue_byte(UCHAR, Service*);
static USHORT service_add_one(USHORT i);
static USHORT service_empty(Service* service);
static USHORT service_full(Service* service);
static void service_fork(ThreadEntryPoint*, Service*);
#else
static void service_fork(TEXT*, Service*);
#endif
static void service_get(Service*, SCHAR *, USHORT, USHORT, USHORT, USHORT *);
static void service_put(Service*, const SCHAR*, USHORT);
#if !defined(WIN_NT) && !defined(SUPERSERVER)
static void timeout_handler(void* service);
#endif
#if defined(WIN_NT) && !defined(SUPERSERVER)
static USHORT service_read(Service*, SCHAR *, USHORT, USHORT);
#endif

#ifdef DEBUG
THREAD_ENTRY_DECLARE test_thread(THREAD_ENTRY_PARAM);
void test_cmd(USHORT, SCHAR *, TEXT **);
#define TEST_THREAD test_thread
#define TEST_CMD test_cmd
#else
#define TEST_THREAD NULL
#define TEST_CMD NULL
#endif

#ifdef SERVER_SHUTDOWN
static shutdown_fct_t shutdown_fct = 0;
static ULONG shutdown_param = 0L;
#endif

const char* const SPB_SEC_USERNAME = "isc_spb_sec_username";

static MUTX_T svc_mutex[1], thd_mutex[1];

/* Service Functions */
#ifdef SUPERSERVER
#include "../burp/burp_proto.h"
#include "../alice/alice_proto.h"
int main_lock_print();
THREAD_ENTRY_DECLARE main_gstat(THREAD_ENTRY_PARAM arg);
#include "../utilities/gsec/gsec_proto.h"

#define MAIN_GBAK		BURP_main
#define MAIN_GFIX		ALICE_main
#define MAIN_LOCK_PRINT	main_lock_print
#define MAIN_GSTAT		main_gstat
#define MAIN_GSEC		GSEC_main
#else
#define MAIN_GBAK		NULL
#define MAIN_GFIX		NULL
#define MAIN_LOCK_PRINT	NULL
#define MAIN_GSTAT		NULL
#define MAIN_GSEC		NULL
#endif

void SVC_STATUS_ARG(ISC_STATUS*& status, USHORT type, const void* value)
{
	if (value)
	{
		switch (type)
		{
		case isc_arg_number:
			*status++ = type;
			*status++ = reinterpret_cast<ISC_STATUS>(value);
			break;
		case isc_arg_string:
			*status++ = type;
			*status++ = (ISC_STATUS)
			SVC_err_string(static_cast<const char*>(value),
						   strlen(static_cast<const char*>(value)));
			break;
		default:
			break;
		}
	}
}

/* Entries which have a NULL serv_executable field will not fork
   a process on the server, but will establish a valid connection
   which can be used for isc_info_svc calls.

  The format of the structure is:

  isc_action_svc call,
  old service name (for compatibility),
  old cmd-line switches (for compatibility),
  executable to fork (for compatibility),
  thread to execute,
  in use flag (for compatibility)
*/
// And the code modifies this flag in this global array???

static serv_entry services[] =
{

	{ isc_action_max, "print_cache", "-svc", "bin/fb_cache_print", NULL, 0 },
	{ isc_action_max, "print_locks", "-svc", "bin/fb_lock_print", NULL, 0 },
	{ isc_action_max, "start_cache", "-svc", "bin/fb_cache_manager", NULL, 0 },
	{ isc_action_max, "analyze_database", "-svc", "bin/gstat", NULL, 0 },
	{ isc_action_max, "backup", "-svc -b", "bin/gbak",	MAIN_GBAK, 0 },
	{ isc_action_max, "create", "-svc -c", "bin/gbak",	MAIN_GBAK, 0 },
	{ isc_action_max, "restore", "-svc -r", "bin/gbak",	MAIN_GBAK, 0 },
	{ isc_action_max, "gdef", "-svc", "bin/gdef", NULL, 0 },
	{ isc_action_max, "gsec", "-svc", "bin/gsec", NULL, 0 },
	{ isc_action_max, "disable_journal", "-svc -disable", "bin/gjrn", NULL, 0 },
	{ isc_action_max, "dump_journal", "-svc -online_dump", "bin/gjrn", NULL, 0 },
	{ isc_action_max, "enable_journal", "-svc -enable", "bin/gjrn", NULL, 0 },
	{ isc_action_max, "monitor_journal", "-svc -console", "bin/gjrn", NULL, 0 },
	{ isc_action_max, "query_server", NULL, NULL, NULL, 0 },
	{ isc_action_max, "start_journal", "-svc -server", "bin/gjrn", NULL, 0 },
	{ isc_action_max, "stop_cache", "-svc -shut -cache", "bin/gfix", NULL, 0 },
	{ isc_action_max, "stop_journal", "-svc -console", "bin/gjrn", NULL, 0 },
	{ isc_action_max, "anonymous", NULL, NULL, NULL, 0 },

/* NEW VERSION 2 calls, the name field MUST be different from those names above
 */
	{ isc_action_max, "service_mgr", NULL, NULL, NULL, 0 },
	{ isc_action_svc_backup, "Backup Database", NULL, "bin/gbak",	MAIN_GBAK, 0 },
	{ isc_action_svc_restore, "Restore Database", NULL, "bin/gbak",	MAIN_GBAK, 0 },
	{ isc_action_svc_repair, "Repair Database", NULL, "bin/gfix",	MAIN_GFIX, 0 },
	{ isc_action_svc_add_user, "Add User", NULL, "bin/gsec",	MAIN_GSEC, 0 },
	{ isc_action_svc_delete_user, "Delete User", NULL, "bin/gsec",	MAIN_GSEC, 0 },
	{ isc_action_svc_modify_user, "Modify User", NULL, "bin/gsec",	MAIN_GSEC, 0 },
	{ isc_action_svc_display_user, "Display User", NULL, "bin/gsec",	MAIN_GSEC, 0 },
	{ isc_action_svc_properties, "Database Properties", NULL, "bin/gfix",	MAIN_GFIX, 0 },
	{ isc_action_svc_lock_stats, "Lock Stats", NULL, "bin/fb_lock_print",	TEST_THREAD, 0 },
	{ isc_action_svc_db_stats, "Database Stats", NULL, "bin/gstat",	MAIN_GSTAT, 0 },
	{ isc_action_svc_get_ib_log, "Get Log File", NULL, NULL,	SVC_read_ib_log, 0 },
/* actions with no names are undocumented */
	{ isc_action_svc_set_config, NULL, NULL, NULL,	TEST_THREAD, 0 },
	{ isc_action_svc_default_config, NULL, NULL, NULL,	TEST_THREAD, 0 },
	{ isc_action_svc_set_env, NULL, NULL, NULL,	TEST_THREAD, 0 },
	{ isc_action_svc_set_env_lock, NULL, NULL, NULL,	TEST_THREAD, 0 },
	{ isc_action_svc_set_env_msg, NULL, NULL, NULL,	TEST_THREAD, 0 },
	{ 0, NULL, NULL, NULL, NULL, 0 }
};

/* The SERVER_CAPABILITIES_FLAG is used to mark architectural
** differences across servers.  This allows applications like server
** manager to disable features as necessary.
*/

#ifdef SUPERSERVER
const ULONG SERVER_CAPABILITIES	= REMOTE_HOP_SUPPORT | MULTI_CLIENT_SUPPORT | SERVER_CONFIG_SUPPORT;
#  ifdef WIN_NT
const ULONG SERVER_CAPABILITIES_FLAG	= SERVER_CAPABILITIES | QUOTED_FILENAME_SUPPORT;
#  else
const ULONG SERVER_CAPABILITIES_FLAG	= SERVER_CAPABILITIES | NO_SERVER_SHUTDOWN_SUPPORT;
#  endif // WIN_NT
#else // SUPERSERVER
const ULONG SERVER_CAPABILITIES_FLAG	= REMOTE_HOP_SUPPORT | NO_SERVER_SHUTDOWN_SUPPORT;
#endif // SERVER_CAPABILITIES


Service* SVC_attach(USHORT	service_length,
			   const TEXT*	service_name,
			   USHORT	spb_length,
			   const SCHAR*	spb)
{
/**************************************
 *
 *	S V C _ a t t a c h
 *
 **************************************
 *
 * Functional description
 *	Connect to a Firebird service.
 *
 **************************************/
/* If the service name begins with a slash, ignore it. */
	thread_db* tdbb = JRD_get_thread_data();

	if (*service_name == '/' || *service_name == '\\') {
		service_name++;
		if (service_length)
			service_length--;
	}
	
	TEXT misc_buf[512];
	// CVC: Close easy buffer overrun attack
	if (!service_length || service_length >= sizeof(misc_buf)) {
	    service_length = (USHORT) sizeof(misc_buf) - 1;
	}
	strncpy(misc_buf, service_name, (size_t) service_length);
	misc_buf[service_length] = 0;

/* Find the service by looking for an exact match. */
	serv_entry* serv;
	for (serv = services; serv->serv_name; serv++) {
		if (!strcmp(misc_buf, serv->serv_name))
			break;
	}

	if (!serv->serv_name)
#ifdef NOT_USED_OR_REPLACED
		ERR_post(isc_service_att_err, isc_arg_gds, isc_service_not_supported,
				 0);
#else
		ERR_post(isc_service_att_err, isc_arg_gds, isc_svcnotdef,
				 isc_arg_string, SVC_err_string(misc_buf, strlen(misc_buf)),
				 0);
#endif

	JRD_get_thread_data();

/* If anything goes wrong, we want to be able to free any memory
   that may have been allocated. */

	SCHAR* spb_buf = 0;
	TEXT* switches = 0;
	TEXT* misc = 0;
	Service* service = 0;

	try {

/* Insert internal switch SERVICE_THD_PARAM in the service parameter block. */

	const SCHAR* p = spb;
	const SCHAR* const end = spb + spb_length;
	bool cmd_line_found = false;
	while (!cmd_line_found && p < end) {
		switch (*p++) {
		case isc_spb_version1:
		case isc_spb_version:
			p++;
			break;

		case isc_spb_sys_user_name:
		case isc_spb_user_name:
		case isc_spb_password:
		case isc_spb_password_enc:
			{
			const USHORT length = *p++;
			p += length;
			}
			break;

		case isc_spb_command_line:
			cmd_line_found = true;
			break;
		}
	}

	/* dimitr: it looks that we shouldn't execute the below code
	           if the first switch of the command line is "-svc_re",
			   but I couldn't find where such a switch is specified
			   by any of the client tools, so it seems that in fact
			   it's not used at all. Hence I ignore this situation. */
	if (cmd_line_found && p++ < end) {
		USHORT ignored_length = 0;
		if (!strncmp(p, "-svc ", 5))
			ignored_length = 5;
		else if (!strncmp(p, "-svc_thd ", 9))
			ignored_length = 9;
		USHORT param_length = sizeof(SERVICE_THD_PARAM) - 1;
		USHORT spb_buf_length = spb_length + param_length - ignored_length + 1;
		SCHAR* q = spb_buf = (TEXT*) gds__alloc(spb_buf_length + 1);
		if (!q)
			ERR_post(isc_virmemexh, 0);
		memcpy(q, spb, p - spb);
		q += p - spb - 1;
		*q++ += param_length - ignored_length + 1;
		memcpy(q, SERVICE_THD_PARAM, param_length);
		q += param_length;
		*q++ = ' ';
		p += ignored_length;
		memcpy(q, p, end - p);
		spb = spb_buf;
		spb_length = spb_buf_length;
	}

/* Process the service parameter block. */

	if (spb_length > sizeof(misc_buf)) {
		misc = (TEXT *) gds__alloc((SLONG) spb_length);
		if (!misc) {
			ERR_post(isc_virmemexh, 0);
		}
	}
	else {
		misc = misc_buf;
	}

	Serv_param_block options;
	get_options(reinterpret_cast<const UCHAR*>(spb), spb_length, misc, &options);

/* Perhaps checkout the user in the security database. */
	USHORT user_flag;
	if (!strcmp(serv->serv_name, "anonymous")) {
		user_flag = SVC_user_none;
	}
	else {
		if (!options.spb_user_name)
		{
			// user name and password are required while
			// attaching to the services manager
			ERR_post(isc_service_att_err, isc_arg_gds, isc_svcnouser, 0);
		}
		if (options.spb_user_name)
		{
			TEXT name[129]; // unused after retrieved
			int id, group, node_id;
			SecurityDatabase::verifyUser(name, options.spb_user_name,
					                     options.spb_password, options.spb_password_enc,
										 &id, &group, &node_id);
		}

/* Check that the validated user has the authority to access this service */

		if (fb_stricmp(options.spb_user_name, SYSDBA_USER_NAME))
		{
			user_flag = SVC_user_any;
		}
		else {
			user_flag = SVC_user_dba | SVC_user_any;
		}
	}

/* Allocate a buffer for the service switches, if any.  Then move them in. */

	const USHORT len =
		((serv->serv_std_switches) ? strlen(serv->serv_std_switches) : 0) +
		((options.spb_command_line) ? strlen(options.spb_command_line) : 0) +
		1;

	if (len > 1)
	{
		if ((switches = (TEXT *) gds__alloc((SLONG) len)) == NULL)
		{
			/* FREE: by exception handler */
			ERR_post(isc_virmemexh, 0);
		}
	}

	if (switches)
		*switches = 0;
	if (serv->serv_std_switches)
		strcpy(switches, serv->serv_std_switches);
	if (options.spb_command_line && serv->serv_std_switches)
		strcat(switches, " ");
	if (options.spb_command_line)
		strcat(switches, options.spb_command_line);

/* Services operate outside of the context of databases.  Therefore
   we cannot use the JRD allocator. */

//	service = (Service*) gds__alloc((SLONG) (sizeof(Service)));
	service = FB_NEW(*getDefaultMemoryPool()) Service;
/* FREE: by exception handler */
	if (!service)
		ERR_post(isc_virmemexh, 0);

	memset((void *) service, 0, sizeof(Service));

	service->svc_status =
		(ISC_STATUS *) gds__alloc(ISC_STATUS_LENGTH * sizeof(ISC_STATUS));
/* FREE: by exception handler */
	if (!service->svc_status)
		ERR_post(isc_virmemexh, 0);

	memset((void *) service->svc_status, 0,
		   ISC_STATUS_LENGTH * sizeof(ISC_STATUS));

	//service->blk_type = type_svc;
	//service->blk_pool_id = 0;
	//service->blk_length = 0;
	service->svc_service = serv;
	service->svc_resp_ptr = service->svc_resp_buf = NULL;
	service->svc_resp_buf_len = service->svc_resp_len = 0;
	service->svc_flags = serv->serv_executable ? SVC_forked : 0;
	service->svc_switches = switches;
	service->svc_handle = 0;
	service->svc_user_flag = user_flag;
	service->svc_do_shutdown = false;
#ifdef SUPERSERVER
	service->svc_stdout_head = 1;
	service->svc_stdout_tail = SVC_STDOUT_BUFFER_SIZE;
	service->svc_stdout = NULL;
	service->svc_argv = NULL;
#endif
	service->svc_spb_version = options.spb_version;
	if (options.spb_user_name)
		strcpy(service->svc_username, options.spb_user_name);

/* The password will be issued to the service threads on NT since
 * there is no OS authentication.  If the password is not yet
 * encrypted, then encrypt it before saving it (since there is no
 * decrypt function).
 */
	if (options.spb_password) {
		ENC_crypt(service->svc_enc_password, sizeof service->svc_enc_password, 
			options.spb_password, PASSWORD_SALT);
	}

	if (options.spb_password_enc) {
		strcpy(service->svc_enc_password, options.spb_password_enc);
	}

/* If an executable is defined for the service, try to fork a new process.
 * Only do this if we are working with a version 1 service */

#ifndef SUPERSERVER
	if (serv->serv_executable && options.spb_version == isc_spb_version1)
#else
	if (serv->serv_thd && options.spb_version == isc_spb_version1)
#endif
	{
#ifndef SUPERSERVER
		TEXT service_path[MAXPATHLEN];
		gds__prefix(service_path, serv->serv_executable);
		service_fork(service_path, service);
#else
		/* if service is single threaded, only call if not currently running */
		if (serv->serv_in_use == NULL) {	/* No worry for multi-threading */
			service_fork(serv->serv_thd, service);
		}
		else if (!*(serv->serv_in_use)) {
			*(serv->serv_in_use) = true;
			service_fork(serv->serv_thd, service);
		}
		else {
			ERR_post(isc_service_att_err, isc_arg_gds,
					 isc_svc_in_use, isc_arg_string, serv->serv_name, 0);
		}
#endif
	}

	if (spb_buf) {
		gds__free(spb_buf);
	}
	if (misc != misc_buf) {
		gds__free(misc);
	}

	}	// try
	catch (const std::exception&) {
		if (spb_buf) {
			gds__free(spb_buf);
		}
		if (misc && misc != misc_buf) {
			gds__free(misc);
		}
		if (switches) {
			gds__free(switches);
		}
		if (service) {
			if (service->svc_status) {
				gds__free(service->svc_status);
			}
//			gds__free(service);
			delete service;
		}
		throw;
	}

	return service;
}


#if defined(SUPERSERVER) && defined(SERVER_SHUTDOWN)
static THREAD_ENTRY_DECLARE shutdown_thread(THREAD_ENTRY_PARAM arg) {
/**************************************
 *
 *	s h u t d o w n _ t h r e a d
 *
 **************************************
 *
 * Functional description
 *	Shutdown SuperServer. If hangs, server 
 *  is forcely & dirty closed after timeout.
 *
 **************************************/

	THREAD_ENTER();
	JRD_shutdown_all();
	*reinterpret_cast<int*>(arg) = 1;
	return 0;
}
#endif // defined(SUPERSERVER) && defined(SERVER_SHUTDOWN)


void SVC_detach(Service* service)
{
/**************************************
 *
 *	S V C _ d e t a c h
 *
 **************************************
 *
 * Functional description
 *	Close down a service.
 *
 **************************************/

#ifdef SERVER_SHUTDOWN
	if (service->svc_do_shutdown) {
#ifdef SUPERSERVER
		int flShutdownComplete = 0;
		gds__thread_start(shutdown_thread, &flShutdownComplete, 
			THREAD_medium, 0, 0);
		THREAD_EXIT();
		int timeout = 10;	// seconds
		while (timeout--) {
			if (flShutdownComplete)
				break;
			THREAD_SLEEP(1 * 1000);
		}
		if (!flShutdownComplete) {
			gds__log("Forced server shutdown - not all databases closed");
		}
#else // SUPERSERVER
		JRD_shutdown_all();
#endif // SUPERSERVER
		if (shutdown_fct)
				(shutdown_fct) (shutdown_param);
		else
			exit(0);

		/* The shutdown operation is complete, just wait to die */
		while (true);
	}
#endif /* SERVER_SHUTDOWN */

#ifdef SUPERSERVER

/* Mark service as detached. */
/* If service thread is finished, cleanup memory being used by service. */

	SVC_finish(service, SVC_detached);

#else

/* Go ahead and cleanup memory being used by service */
	SVC_cleanup(service);

#endif
}

const TEXT* SVC_err_string(const TEXT* data, USHORT length)
{
/********************************************
 *
 *   S V C _ e r r _ s t r i n g
 *
 ********************************************
 *
 * Functional Description:
 *     Uses ERR_string to save string data for the
 *     status vector
 ********************************************/
	return ERR_string(data, length);
}


#ifdef SERVER_SHUTDOWN
void SVC_shutdown_init(shutdown_fct_t fptr,
			ULONG param)
{
/**************************************
 *
 *	S V C _ s h u t d o w n _ i n i t
 *
 **************************************
 *
 * Functional description
 *	Set the global function pointer to the shutdown function.
 *
 **************************************/

	shutdown_fct = fptr;
	shutdown_param = param;
}
#endif // SERVER_SHUTDOWN


#ifdef SUPERSERVER
void SVC_fprintf(Service* service, const SCHAR* format, ...)
{
/**************************************
 *
 *	S V C _ f p r i n t f
 *
 **************************************/
/* Ensure that service is not detached. */
	if (!(service->svc_flags & SVC_detached)) {
		va_list arglist;
		char buf[1000];
		ULONG i = 0;

		va_start(arglist, format);
		// CVC: No bounds checking on "buf"; we assume everything fits in it.
		// Where is that limit documented??? I changed to vsnprintf
		//n = vsprintf(buf, format, arglist);
		int n = VSNPRINTF(buf, sizeof(buf), format, arglist);
		va_end(arglist);
		// Nothing is said if this answer generated by service_enqueue_bytes
		// should be null terminated always. Given the old code that called
		// vsprintf, it's clear that '\0' was't sent, so in the new code,
		// n is adjusted in case of insufficient buffer to behave the same.
		// Changed ULONG to int, since this is the official type of the
		// printf family and makes negative test natural.
		// We could send a truncated answer in this case, since vsnprintf
		// returns negative when buffer is not enough. Since the old code
		// didn't send anything after an error, enable this to test if sending
		// a partial result would crash the receiver. Failure may happen before
		// reaching end of buffer for other reasons, so we might transfer trash.
		//if (n < 0)
		//{
		//    n = sizeof(buf) - 1;
		//    buf[n] = 0;
		//}
		while (n > 0 && !(service->svc_flags & SVC_detached)) {
			service_enqueue_byte(buf[i], service);
			n--;
			i++;
		}
	}
}


void SVC_putc(Service* service, const UCHAR ch)
{
/**************************************
 *
 *	S V C _ p u t c
 *
 **************************************/
/* Ensure that service is not detached. */
	if (!(service->svc_flags & SVC_detached))
		service_enqueue_byte(ch, service);
}

//____________________________________________________________
//
//	Routine which is used by the service for calling back when there is output.
//
// CVC: Now we are using a SLONG to hold a pointer to the Service!

int SVC_output(Service* output_data, const UCHAR* output_buf)
{
	SVC_fprintf(output_data, "%s", output_buf);

	return 0;
}
#endif /*SUPERSERVER*/

ISC_STATUS SVC_query2(Service* service,
					  thread_db* tdbb,
					  USHORT send_item_length,
					  const SCHAR* send_items,
					  USHORT recv_item_length,
					  const SCHAR* recv_items,
					  USHORT buffer_length,
					  SCHAR* info)
{
/**************************************
 *
 *	S V C _ q u e r y 2
 *
 **************************************
 *
 * Functional description
 *	Provide information on service object
 *
 **************************************/
	SCHAR item;
	char buffer[MAXPATHLEN];
	USHORT l, length, version, get_flags;

	THREAD_EXIT();

/* Setup the status vector */
	ISC_STATUS* status = tdbb->tdbb_status_vector;
	*status++ = isc_arg_gds;

/* Process the send portion of the query first. */

	USHORT timeout = 0;
	const SCHAR* items = send_items;
	const SCHAR* const end_items = items + send_item_length;
	while (items < end_items && *items != isc_info_end)
	{
		switch ((item = *items++))
		{
		case isc_info_end:
			break;

		default:
			if (items + 2 <= end_items) {
				l =
					(USHORT) gds__vax_integer(reinterpret_cast<
											  const UCHAR*>(items), 2);
				items += 2;
				if (items + l <= end_items) {
					switch (item) {
					case isc_info_svc_line:
						service_put(service, items, l);
						break;
					case isc_info_svc_message:
						service_put(service, items - 3, l + 3);
						break;
					case isc_info_svc_timeout:
						timeout =
							(USHORT) gds__vax_integer(reinterpret_cast<
													  const UCHAR*>(items), l);
						break;
					case isc_info_svc_version:
						version =
							(USHORT) gds__vax_integer(reinterpret_cast<
													  const UCHAR*>(items), l);
						break;
					}
				}
				items += l;
			}
			else
				items += 2;
			break;
		}
	}

/* Process the receive portion of the query now. */

	const SCHAR* const end = info + buffer_length;

	items = recv_items;
	const SCHAR* const end_items2 = items + recv_item_length;
	while (items < end_items2 && *items != isc_info_end)
	{
		/*
		   if we attached to the "anonymous" service we allow only following queries:

		   isc_info_svc_get_config     - called from within remote/ibconfig.cpp
		   isc_info_svc_dump_pool_info - called from within utilities/print_pool.cpp
//		   isc_info_svc_user_dbpath    - called from within utilities/security.cpp
		 */
		if (service->svc_user_flag == SVC_user_none)
		{
			switch (*items)
			{
			case isc_info_svc_get_config:
			case isc_info_svc_dump_pool_info:
//			case isc_info_svc_user_dbpath:
				break;
			default:
				ERR_post(isc_bad_spb_form, 0);
				break;
			}
		}

		switch ((item = *items++))
		{
		case isc_info_end:
			break;

#ifdef SERVER_SHUTDOWN
		case isc_info_svc_svr_db_info:
			{
				UCHAR dbbuf[1024];
				USHORT num_dbs = 0;
				USHORT num_att = 0;

				*info++ = item;
				TEXT* const ptr =
					JRD_num_attachments(reinterpret_cast<char*>(dbbuf),
										sizeof(dbbuf), JRD_info_dbnames,
										&num_att, &num_dbs);
				/* Move the number of attachments into the info buffer */
				if (!ck_space_for_numeric(info, end))
					return 0;
				*info++ = isc_spb_num_att;
				ADD_SPB_NUMERIC(info, num_att);

				/* Move the number of databases in use into the info buffer */
				if (!ck_space_for_numeric(info, end))
					return 0;
				*info++ = isc_spb_num_db;
				ADD_SPB_NUMERIC(info, num_dbs);

				/* Move db names into the info buffer */
				TEXT* ptr2 = ptr;
				if (ptr2) {
					USHORT num = (USHORT) isc_vax_integer(ptr2, sizeof(USHORT));
					fb_assert(num == num_dbs);
					ptr2 += sizeof(USHORT);
					for (; num; num--) {
						length =
							(USHORT) isc_vax_integer(ptr2, sizeof(USHORT));
						ptr2 += sizeof(USHORT);
						if (!
							(info =
							 INF_put_item(isc_spb_dbname, length, ptr2, info,
										  end)))
						{
							// CVC: Shouldn't this place try to free ptr
							// if it's different than dbbuf, too?
							THREAD_ENTER();
							return 0;
						}
						ptr2 += length;
					}

					if (ptr != reinterpret_cast<TEXT*>(dbbuf))
						gds__free(ptr);	/* memory has been allocated by
										   JRD_num_attachments() */
				}

				if (info < end)
					*info++ = isc_info_flag_end;
			}

			break;

		case isc_info_svc_svr_online:
			*info++ = item;
			if (service->svc_user_flag & SVC_user_dba) {
				service->svc_do_shutdown = false;
				WHY_set_shutdown(FALSE);
			}
			else
				need_admin_privs(&status, "isc_info_svc_svr_online");
			break;

		case isc_info_svc_svr_offline:
			*info++ = item;
			if (service->svc_user_flag & SVC_user_dba) {
				service->svc_do_shutdown = true;
				WHY_set_shutdown(TRUE);
			}
			else
				need_admin_privs(&status, "isc_info_svc_svr_offline");
			break;
#endif /* SERVER_SHUTDOWN */

			/* The following 3 service commands (or items) stuff the response
			   buffer 'info' with values of environment variable INTERBASE,
			   INTERBASE_LOCK or INTERBASE_MSG. If the environment variable
			   is not set then default value is returned.
			 */
		case isc_info_svc_get_env:
		case isc_info_svc_get_env_lock:
		case isc_info_svc_get_env_msg:
			switch (item) {
			case isc_info_svc_get_env:
				gds__prefix(buffer, "");
				break;
			case isc_info_svc_get_env_lock:
				gds__prefix_lock(buffer, "");
				break;
			case isc_info_svc_get_env_msg:
				gds__prefix_msg(buffer, "");
			}

			/* Note: it is safe to use strlen to get a length of "buffer"
			   because gds_prefix[_lock|_msg] return a zero-terminated
			   string
			 */
			info = INF_put_item(item, strlen(buffer), buffer, info, end);
			if (!info) {
				THREAD_ENTER();
				return 0;
			}
			break;

#ifdef SUPERSERVER
		case isc_info_svc_dump_pool_info:
			{
				char fname[MAXPATHLEN];
				size_t length2 = isc_vax_integer(items, sizeof(USHORT));
				if (length2 >= sizeof(fname))
					length2 = sizeof(fname) - 1; // truncation
				items += sizeof(USHORT);
				strncpy(fname, items, length2);
				fname[length2] = 0;
				JRD_print_all_counters(fname);
				break;
			}
#endif
/*
		case isc_info_svc_get_config:
			// TODO: iterate through all integer-based config values
			//		 and return them to the client
			break;

		case isc_info_svc_default_config:
			*info++ = item;
			if (service->svc_user_flag & SVC_user_dba) {
				THREAD_ENTER();
				// TODO: reset the config values to defaults
				THREAD_EXIT();
			}
			else
				need_admin_privs(&status, "isc_info_svc_default_config");
			break;

		case isc_info_svc_set_config:
			*info++ = item;
			if (service->svc_user_flag & SVC_user_dba) {
				THREAD_ENTER();
				// TODO: set the config values
				THREAD_EXIT();
			}
			else {
				need_admin_privs(&status, "isc_info_svc_set_config");
			}
			break;
*/
		case isc_info_svc_version:
			/* The version of the service manager */
			if (!ck_space_for_numeric(info, end))
				return 0;
			*info++ = item;
			ADD_SPB_NUMERIC(info, SERVICE_VERSION);
			break;

		case isc_info_svc_capabilities:
			/* bitmask defining any specific architectural differences */
			if (!ck_space_for_numeric(info, end))
				return 0;
			*info++ = item;
			ADD_SPB_NUMERIC(info, SERVER_CAPABILITIES_FLAG);
			break;

		case isc_info_svc_running:
			/* Returns the status of the flag SVC_thd_running */
			if (!ck_space_for_numeric(info, end))
				return 0;
			*info++ = item;
			if (service->svc_flags & SVC_thd_running)
				ADD_SPB_NUMERIC(info, TRUE)
			else
				ADD_SPB_NUMERIC(info, FALSE)

			break;

		case isc_info_svc_server_version:
			/* The version of the server engine */
			info = INF_put_item(item, strlen(GDS_VERSION), GDS_VERSION, info, end);
			if (!info) {
				THREAD_ENTER();
				return 0;
			}
			break;

		case isc_info_svc_implementation:
			/* The server implementation - e.g. Interbase/sun4 */
			isc_format_implementation(IMPLEMENTATION, sizeof(buffer), buffer,
									  0, 0, NULL);
			info = INF_put_item(item, strlen(buffer), buffer, info, end);
			if (!info) {
				THREAD_ENTER();
				return 0;
			}

			break;


		case isc_info_svc_user_dbpath:
			/* The path to the user security database (security.fdb) */
			SecurityDatabase::getPath(buffer);

			if (!(info = INF_put_item(item, strlen(buffer), buffer,
									  info, end)))
			{
				THREAD_ENTER();
				return 0;
			}
			break;

		case isc_info_svc_response:
			service->svc_resp_len = 0;
			if (info + 4 >= end) {
				*info++ = isc_info_truncated;
				break;
			}
			service_put(service, &item, 1);
			service_get(service, &item, 1, GET_BINARY, 0, &length);
			service_get(service, buffer, 2, GET_BINARY, 0, &length);
			l =
				(USHORT) gds__vax_integer(reinterpret_cast<
										  UCHAR*>(buffer), 2);
			length = MIN(end - (info + 5), l);
			service_get(service, info + 3, length, GET_BINARY, 0, &length);
			info = INF_put_item(item, length, info + 3, info, end);
			if (length != l) {
				*info++ = isc_info_truncated;
				l -= length;
				if (l > service->svc_resp_buf_len) {
					THREAD_ENTER();
					if (service->svc_resp_buf)
						gds__free(service->svc_resp_buf);
					service->svc_resp_buf = (UCHAR *) gds__alloc((SLONG) l);
					/* FREE: in SVC_detach() */
					if (!service->svc_resp_buf) {	/* NOMEM: */
						DEV_REPORT("SVC_query: out of memory");
						/* NOMEM: not really handled well */
						l = 0;	/* set the length to zero */
					}
					service->svc_resp_buf_len = l;
					THREAD_EXIT();
				}
				service_get(service,
							reinterpret_cast<char*>(service->svc_resp_buf),
							l, GET_BINARY, 0, &length);
				service->svc_resp_ptr = service->svc_resp_buf;
				service->svc_resp_len = l;
			}
			break;

		case isc_info_svc_response_more:
			if ( (l = length = service->svc_resp_len) )
				length = MIN(end - (info + 5), l);
			if (!
				(info =
				 INF_put_item(item, length,
							  reinterpret_cast<const char*>
								  (service->svc_resp_ptr), 
							  info, end))) 
			{
				THREAD_ENTER();
				return 0;
			}
			service->svc_resp_ptr += length;
			service->svc_resp_len -= length;
			if (length != l)
				*info++ = isc_info_truncated;
			break;

		case isc_info_svc_total_length:
			service_put(service, &item, 1);
			service_get(service, &item, 1, GET_BINARY, 0, &length);
			service_get(service, buffer, 2, GET_BINARY, 0, &length);
			l = (USHORT) gds__vax_integer(reinterpret_cast<
										  UCHAR*>(buffer), 2);
			service_get(service, buffer, l, GET_BINARY, 0, &length);
			if (!(info = INF_put_item(item, length, buffer, info, end))) {
				THREAD_ENTER();
				return 0;
			}
			break;

		case isc_info_svc_line:
		case isc_info_svc_to_eof:
		case isc_info_svc_limbo_trans:
		case isc_info_svc_get_users:
			if (info + 4 >= end) {
				*info++ = isc_info_truncated;
				break;
			}

			if (item == isc_info_svc_line)
				get_flags = GET_LINE;
			else if (item == isc_info_svc_to_eof)
				get_flags = GET_EOF;
			else
				get_flags = GET_BINARY;

			service_get(service, info + 3, end - (info + 5), get_flags,
						timeout, &length);

			/* If the read timed out, return the data, if any, & a timeout
			   item.  If the input buffer was not large enough
			   to store a read to eof, return the data that was read along
			   with an indication that more is available. */

			if (!(info = INF_put_item(item, length, info + 3, info, end))) {
				THREAD_ENTER();
				return 0;
			}

			if (service->svc_flags & SVC_timeout)
			{
				*info++ = isc_info_svc_timeout;
			}
			else
			{
				if (!length && !(service->svc_flags & SVC_finished))
				{
					*info++ = isc_info_data_not_ready;
				}
				else
				{
					if (item == isc_info_svc_to_eof &&
						!(service->svc_flags & SVC_finished))
					{
						*info++ = isc_info_truncated;
					}
				}
			}
			break;
		}

		if (service->svc_user_flag == SVC_user_none)
			break;
	}

	if (info < end)
		*info = isc_info_end;


	if (!(service->svc_flags & SVC_thd_running))
	{
		SVC_finish(service, SVC_finished);
	}

	THREAD_ENTER();
	return tdbb->tdbb_status_vector[1];
}

void SVC_query(Service*		service,
			   USHORT	send_item_length,
			   const SCHAR*	send_items,
			   USHORT	recv_item_length,
			   const SCHAR*	recv_items,
			   USHORT	buffer_length,
			   SCHAR*	info)
{
/**************************************
 *
 *	S V C _ q u e r y
 *
 **************************************
 *
 * Functional description
 *	Provide information on service object.
 *
 **************************************/
	SCHAR item, *p;
	char buffer[256];
	TEXT PathBuffer[MAXPATHLEN];
	USHORT l, length, version, get_flags;

	THREAD_EXIT();

/* Process the send portion of the query first. */

	USHORT timeout = 0;
	const SCHAR* items = send_items;
	const SCHAR* const end_items = items + send_item_length;
	while (items < end_items && *items != isc_info_end)
	{
		switch ((item = *items++))
		{
		case isc_info_end:
			break;

		default:
			if (items + 2 <= end_items)
			{
				l = (USHORT) gds__vax_integer(reinterpret_cast<const UCHAR*>(items), 2);
				items += 2;
				if (items + l <= end_items)
				{
					switch (item) {
					case isc_info_svc_line:
						service_put(service, items, l);
						break;
					case isc_info_svc_message:
						service_put(service, items - 3, l + 3);
						break;
					case isc_info_svc_timeout:
						timeout =
							(USHORT) gds__vax_integer(reinterpret_cast<const UCHAR*>(items), l);
						break;
					case isc_info_svc_version:
						version =
							(USHORT) gds__vax_integer(reinterpret_cast<const UCHAR*>(items), l);
						break;
					}
				}
				items += l;
			}
			else
				items += 2;
			break;
		}
	}

/* Process the receive portion of the query now. */

	const SCHAR* const end = info + buffer_length;

	items = recv_items;
	const SCHAR* const end_items2 = items + recv_item_length;
	while (items < end_items2 && *items != isc_info_end)
	{
		switch ((item = *items++))
		{
		case isc_info_end:
			break;

#ifdef SERVER_SHUTDOWN
		case isc_info_svc_svr_db_info:
			{
				USHORT num_att = 0;
				USHORT num_dbs = 0;
				JRD_num_attachments(NULL, 0, 0, &num_att, &num_dbs);
				length = INF_convert(num_att, buffer);
				info = INF_put_item(item,
									length,
									buffer,
									info,
									end);
				if (!info) {
					THREAD_ENTER();
					return;
				}
				length = INF_convert(num_dbs, buffer);
				info = INF_put_item(item,
									length,
									buffer,
									info,
									end);
				if (!info) {
					THREAD_ENTER();
					return;
				}
			}
			break;

		case isc_info_svc_svr_online:
			*info++ = item;
			if (service->svc_user_flag & SVC_user_dba) {
				service->svc_do_shutdown = false;
				WHY_set_shutdown(FALSE);
				*info++ = 0;	/* Success */
			}
			else
				*info++ = 2;	/* No user authority */
			break;

		case isc_info_svc_svr_offline:
			*info++ = item;
			if (service->svc_user_flag & SVC_user_dba) {
				service->svc_do_shutdown = true;
				WHY_set_shutdown(TRUE);
				*info++ = 0;	/* Success */
			}
			else
				*info++ = 2;	/* No user authority */
			break;
#endif /* SERVER_SHUTDOWN */

			/* The following 3 service commands (or items) stuff the response
			   buffer 'info' with values of environment variable INTERBASE,
			   INTERBASE_LOCK or INTERBASE_MSG. If the environment variable
			   is not set then default value is returned.
			 */
		case isc_info_svc_get_env:
		case isc_info_svc_get_env_lock:
		case isc_info_svc_get_env_msg:
			switch (item) {
			case isc_info_svc_get_env:
				gds__prefix(PathBuffer, "");
				break;
			case isc_info_svc_get_env_lock:
				gds__prefix_lock(PathBuffer, "");
				break;
			case isc_info_svc_get_env_msg:
				gds__prefix_msg(PathBuffer, "");
			}

			/* Note: it is safe to use strlen to get a length of "buffer"
			   because gds_prefix[_lock|_msg] return a zero-terminated
			   string
			 */
			if (!(info = INF_put_item(item, strlen(PathBuffer),
									  PathBuffer, info, end)))
			{
				THREAD_ENTER();
				return;
			}
			break;

#ifdef SUPERSERVER
		case isc_info_svc_dump_pool_info:
			{
				char fname[MAXPATHLEN];
				size_t length2 = isc_vax_integer(items, sizeof(USHORT));
				if (length2 >= sizeof(fname))
					length2 = sizeof(fname) - 1; // truncation
				items += sizeof(USHORT);
				strncpy(fname, items, length2);
				fname[length2] = 0;
				JRD_print_all_counters(fname);
				break;
			}
#endif
/*
		case isc_info_svc_get_config:
			// TODO: iterate through all integer-based config values
			//		 and return them to the client
			break;

		case isc_info_svc_default_config:
			*info++ = item;
			if (service->svc_user_flag & SVC_user_dba) {
				THREAD_ENTER();
				// TODO: reset the config values to defaults
				THREAD_EXIT();
			}
			else
				need_admin_privs(&status, "isc_info_svc_default_config");
			break;

		case isc_info_svc_set_config:
			*info++ = item;
			if (service->svc_user_flag & SVC_user_dba) {
				THREAD_ENTER();
				// TODO: set the config values
				THREAD_EXIT();
			}
			else {
				need_admin_privs(&status, "isc_info_svc_set_config");
			}
			break;
*/
		case isc_info_svc_version:
			/* The version of the service manager */

			length =
				INF_convert(SERVICE_VERSION, buffer);
			if (!
				(info =
				 INF_put_item(item, length, buffer, info, end)))
			{
				THREAD_ENTER();
				return;
			}
			break;

		case isc_info_svc_capabilities:
			/* bitmask defining any specific architectural differences */

			length =
				INF_convert(SERVER_CAPABILITIES_FLAG, buffer);
			if (!
				(info =
				 INF_put_item(item, length, buffer, info, end)))
			{
				THREAD_ENTER();
				return;
			}
			break;

		case isc_info_svc_server_version:
			{
				/* The version of the server engine */

				p = buffer;
				*p++ = 1;			/* Count */
				*p++ = sizeof(GDS_VERSION) - 1;
				for (const TEXT* gvp = GDS_VERSION; *gvp; p++, gvp++)
					*p = *gvp;
				if (!(info = INF_put_item(item, p - buffer, buffer, info, end)))
				{
					THREAD_ENTER();
					return;
				}
				break;
			}

		case isc_info_svc_implementation:
			/* The server implementation - e.g. Interbase/sun4 */

			p = buffer;
			*p++ = 1;			/* Count */
			*p++ = IMPLEMENTATION;
			if (!(info = INF_put_item(item, p - buffer, buffer, info, end)))
			{
				THREAD_ENTER();
				return;
			}
			break;


		case isc_info_svc_user_dbpath:
			/* The path to the user security database (security.fdb) */
			SecurityDatabase::getPath(buffer);

			if (!(info = INF_put_item(item, strlen(buffer), buffer, info, end)))
			{
				THREAD_ENTER();
				return;
			}
			break;

		case isc_info_svc_response:
			service->svc_resp_len = 0;
			if (info + 4 > end)
			{
				*info++ = isc_info_truncated;
				break;
			}
			service_put(service, &item, 1);
			service_get(service, &item, 1, GET_BINARY, 0, &length);
			service_get(service, buffer, 2, GET_BINARY, 0, &length);
			l = (USHORT) gds__vax_integer(reinterpret_cast<UCHAR*>(buffer), 2);
			length = MIN(end - (info + 4), l);
			service_get(service, info + 3, length, GET_BINARY, 0, &length);
			info = INF_put_item(item, length, info + 3, info, end);
			if (length != l)
			{
				*info++ = isc_info_truncated;
				l -= length;
				if (l > service->svc_resp_buf_len)
				{
					THREAD_ENTER();
					if (service->svc_resp_buf)
						gds__free(service->svc_resp_buf);
					service->svc_resp_buf = (UCHAR *) gds__alloc((SLONG) l);
					/* FREE: in SVC_detach() */
					if (!service->svc_resp_buf)
					{	/* NOMEM: */
						DEV_REPORT("SVC_query: out of memory");
						/* NOMEM: not really handled well */
						l = 0;	/* set the length to zero */
					}
					service->svc_resp_buf_len = l;
					THREAD_EXIT();
				}
				service_get(service,
							reinterpret_cast<char*>(service->svc_resp_buf),
							l,
							GET_BINARY,
							0,
							&length);
				service->svc_resp_ptr = service->svc_resp_buf;
				service->svc_resp_len = l;
			}
			break;

		case isc_info_svc_response_more:
			if ( (l = length = service->svc_resp_len) )
				length = MIN(end - (info + 4), l);
			if (!(info = INF_put_item(item,
									length,
									reinterpret_cast<const char*>
										(service->svc_resp_ptr),
									info,
									end)))
			{
				THREAD_ENTER();
				return;
			}
			service->svc_resp_ptr += length;
			service->svc_resp_len -= length;
			if (length != l)
				*info++ = isc_info_truncated;
			break;

		case isc_info_svc_total_length:
			service_put(service, &item, 1);
			service_get(service, &item, 1, GET_BINARY, 0, &length);
			service_get(service, buffer, 2, GET_BINARY, 0, &length);
			l = (USHORT) gds__vax_integer(reinterpret_cast<UCHAR*>(buffer), 2);
			service_get(service, buffer, l, GET_BINARY, 0, &length);
			if (!(info = INF_put_item(item, length, buffer, info, end)))
			{
				THREAD_ENTER();
				return;
			}
			break;

		case isc_info_svc_line:
		case isc_info_svc_to_eof:
			if (info + 4 > end)
			{
				*info++ = isc_info_truncated;
				break;
			}
			get_flags = (item == isc_info_svc_line) ? GET_LINE : GET_EOF;
			service_get(service, info + 3, end - (info + 4), get_flags,
						timeout, &length);

			/* If the read timed out, return the data, if any, & a timeout
			   item.  If the input buffer was not large enough
			   to store a read to eof, return the data that was read along
			   with an indication that more is available. */

			info = INF_put_item(item, length, info + 3, info, end);

			if (service->svc_flags & SVC_timeout)
				*info++ = isc_info_svc_timeout;
			else
			{
				if (!length && !(service->svc_flags & SVC_finished))
					*info++ = isc_info_data_not_ready;
				else
					if (item == isc_info_svc_to_eof &&
						!(service->svc_flags & SVC_finished))
						*info++ = isc_info_truncated;
			}
			break;
		}
	}

	if (info < end)
	{
		*info = isc_info_end;
	}

	if (!(service->svc_flags & SVC_thd_running))
	{
		SVC_finish(service, SVC_finished);
	}

	THREAD_ENTER();
}


void* SVC_start(Service* service, USHORT spb_length, const SCHAR* spb)
{
/**************************************
 *
 *	S V C _ s t a r t
 *
 **************************************
 *
 * Functional description
 *      Start a Firebird service
 *
 **************************************/
 	
/* 	NOTE: The parameter RESERVED must not be used
 *	for any purpose as there are networking issues
 *	involved (as with any handle that goes over the
 *	network).  This parameter will be implemented at
 *	a later date.
 */  

	isc_resv_handle reserved = (isc_resv_handle)0;	/* Reserved for future functionality */

/* The name of the service is the first element of the buffer */
	const USHORT svc_id = *spb;
	serv_entry* serv;
	for (serv = services; serv->serv_action; serv++)
		if (serv->serv_action == svc_id)
			break;

	if (!serv->serv_name)
		ERR_post(isc_service_att_err, isc_arg_gds, isc_service_not_supported, 0);

/* currently we do not use "anonymous" service for any purposes but
   isc_service_query() */
	if (service->svc_user_flag == SVC_user_none)
		ERR_post(isc_bad_spb_form, 0);

	THD_MUTEX_LOCK(thd_mutex);
	if (service->svc_flags & SVC_thd_running) {
		THD_MUTEX_UNLOCK(thd_mutex);
		ERR_post(isc_svc_in_use, isc_arg_string,
				 SVC_err_string(serv->serv_name, strlen(serv->serv_name)),
				 0);
	}
	else {
		/* Another service may have been started with this service block.  If so,
		 * we must reset the service flags.
		 */
		if (!(service->svc_flags & SVC_detached))
			service->svc_flags = 0;
		service->svc_flags |= SVC_thd_running;
		if (service->svc_switches) {
			gds__free(service->svc_switches);
			service->svc_switches = NULL;
		}
	}
	THD_MUTEX_UNLOCK(thd_mutex);

	thread_db* tdbb = JRD_get_thread_data();

	try {

/* Only need to add username and password information to those calls which need
 * to make a database connection
 */
 	USHORT opt_switch_len = 0;
 	
	if (*spb == isc_action_svc_backup ||
		*spb == isc_action_svc_restore ||
		*spb == isc_action_svc_repair ||
		*spb == isc_action_svc_add_user ||
		*spb == isc_action_svc_delete_user ||
		*spb == isc_action_svc_modify_user ||
		*spb == isc_action_svc_display_user ||
		*spb == isc_action_svc_db_stats ||
		*spb == isc_action_svc_properties)
	{
		/* the user issued a username when connecting to the service so
		 * add the length of the username and switch to new_spb_length
		 */
		TEXT* tmp_ptr = NULL;

		if (*service->svc_username)
			opt_switch_len +=
				(strlen(service->svc_username) + 1 + sizeof(USERNAME_SWITCH));

		/* the user issued a password when connecting to the service so add
		 * the length of the password and switch to new_spb_length
		 */
		if (*service->svc_enc_password)
			opt_switch_len +=
				(strlen(service->svc_enc_password) + 1 +
				 sizeof(PASSWORD_SWITCH));

		bool flag_spb_options = false;
		/* If svc_switches is not used -- call a command-line parsing utility */
		if (!service->svc_switches) {
			conv_switches(spb_length, opt_switch_len, spb,
						  &service->svc_switches);
		}
		else {
			/* Command line options (isc_spb_options) is used.
			 * Currently the only case in which it might happen is -- gbak utility
			 * is called with a "-server" switch.
			 */
			flag_spb_options = true;

			tmp_ptr = (TEXT *)
				gds__alloc((SLONG)
						   (strlen(service->svc_switches) + 1 +
							opt_switch_len + 1));
			if (!tmp_ptr)		/* NOMEM: */
				ERR_post(isc_virmemexh, 0);
		}

		/* add the username and password to the end of svc_switches if needed */
		if (service->svc_switches) {
			if (flag_spb_options)
				strcpy(tmp_ptr, service->svc_switches);

			if (*service->svc_username) {
				if (!flag_spb_options)
					sprintf(service->svc_switches, "%s %s %s",
							service->svc_switches, USERNAME_SWITCH,
							service->svc_username);
				else
					sprintf(tmp_ptr, "%s %s %s", tmp_ptr, USERNAME_SWITCH,
							service->svc_username);
			}

			if (*service->svc_enc_password) {
				if (!flag_spb_options)
					sprintf(service->svc_switches, "%s %s %s",
							service->svc_switches, PASSWORD_SWITCH,
							service->svc_enc_password);
				else
					sprintf(tmp_ptr, "%s %s %s", tmp_ptr, PASSWORD_SWITCH,
							service->svc_enc_password);
			}

			if (flag_spb_options) {
				gds__free(service->svc_switches);
				service->svc_switches = tmp_ptr;
			}
		}
	}
	else {
		/* If svc_switches is not used -- call a command-line parsing utility */
		if (!service->svc_switches) {
			conv_switches(spb_length, opt_switch_len, spb,
						  &service->svc_switches);
		}
		else {
			fb_assert(service->svc_switches == NULL);
		}
	}
/* All services except for get_ib_log require switches */
	if (service->svc_switches == NULL && *spb != isc_action_svc_get_ib_log)
		ERR_post(isc_bad_spb_form, 0);

#ifndef SUPERSERVER
	TEXT service_path[MAXPATHLEN];

	if (serv->serv_executable) {
		gds__prefix(service_path, serv->serv_executable);
		service->svc_flags = SVC_forked;
		service_fork(service_path, service);
	}

#else

/* Break up the command line into individual arguments. */
	if (service->svc_switches)
	{
		USHORT argc;
		TEXT* p;
		for (argc = 2, p = service->svc_switches; *p;) {
			if (*p == ' ') {
				argc++;
				while (*p == ' ')
					p++;
			}
			else {
				if (*p == SVC_TRMNTR) {
					while (*p++ && *p != SVC_TRMNTR);
					fb_assert(*p == SVC_TRMNTR);
				}
				p++;
			}
		}

		service->svc_argc = argc;

		TEXT** arg = (TEXT **) gds__alloc((SLONG) ((argc + 1) * sizeof(TEXT *)));
		/*
		 * the service block can be reused hence free a memory from the
		 * previous usage if any.
		 */
		if (service->svc_argv)
			gds__free(service->svc_argv);
		service->svc_argv = arg;
		/* FREE: at SVC_detach() - Possible memory leak if ERR_post() occurs */
		if (!arg)				/* NOMEM: */
			ERR_post(isc_virmemexh, 0);

		*arg++ = (TEXT *)(serv->serv_thd);
		const TEXT* q = p = service->svc_switches;

		while (*q == ' ')
			q++;

		while (*q) {
			*arg = p;
			while (*p = *q++) {
				if (*p == ' ')
					break;

				if (*p == SVC_TRMNTR) {
					*arg = ++p;	/* skip terminator */
					while (*p = *q++)
						/* If *q points to the last argument, then terminate the argument */
						if ((*q == 0 || *q == ' ') && *p == SVC_TRMNTR) {
							*p = '\0';
							break;
						}
						else
							p++;
				}

				if (*p == '\\' && *q == ' ') {
					*p = ' ';
					q++;
				}
				p++;
			}
			arg++;
			if (!*p)
				break;
			*p++ = '\0';
			while (*q == ' ')
				q++;
		}
		*arg = NULL;
	}

/*
 * the service block can be reused hence free a memory from the
 * previous usage as well as init a status vector.
 */

	memset((void *) service->svc_status, 0,
		   ISC_STATUS_LENGTH * sizeof(ISC_STATUS));

	if (service->svc_stdout)
		gds__free(service->svc_stdout);

	service->svc_stdout = (UCHAR*)gds__alloc((SLONG) SVC_STDOUT_BUFFER_SIZE + 1);
/* FREE: at SVC_detach() */
	if (!service->svc_stdout)	/* NOMEM: */
	{
		ERR_post(isc_virmemexh, 0);
	}

	if (serv->serv_thd) {
#pragma FB_COMPILER_MESSAGE("Fix! Probable bug!")
		event_t* evnt_ptr =
			reinterpret_cast<event_t*>(&(service->svc_start_event));

		THREAD_EXIT();
		/* create an event for the service.  The event will be signaled once the
		 * particular service has reached a point in which it can start to return
		 * information to the client.  This will allow isc_service_start to
		 * include in its status vector information about the service's ability to
		 * start.  This is needed since gds__thread_start will almost always
		 * succeed.
		 */
		ISC_event_init(evnt_ptr, 0, 0);
		SLONG count = ISC_event_clear(evnt_ptr);

		gds__thread_start(serv->serv_thd, service, THREAD_medium, 0,
						  (void *) &service->svc_handle);

		/* check for the service being detached.  This will prevent the thread
		 * from waiting infinitely if the client goes away
		 */
		while (!(service->svc_flags & SVC_detached)) {
			if (ISC_event_wait(1, &evnt_ptr, &count, 60 * 1000, NULL, 0))
			{
				continue;
			}
			else {				/* the event was posted */
				break;
			}
		}

		ISC_event_fini(evnt_ptr);
		THREAD_ENTER();
	}
	else
	{
		ERR_post(isc_svcnotdef,
				isc_arg_string,
				SVC_err_string(serv->serv_name, strlen(serv->serv_name)),
				0);
	}

#endif /* SUPERSERVER */

	}	// try
	catch (const std::exception&) {
		if (service->svc_switches) {
			gds__free(service->svc_switches);
		}
		if (service) {
//			gds__free(service);
			delete service;
		}
		throw;
	}

	return reinterpret_cast<void*>(reserved);
}


THREAD_ENTRY_DECLARE SVC_read_ib_log(THREAD_ENTRY_PARAM arg)
{
/**************************************
 *
 *      S V C _ r e a d _ i b _ l o g
 *
 **************************************
 *
 * Functional description
 *   Service function which reads the Firebird
 *   log file into the service buffers.
 *
 **************************************/
	bool svc_started = false;
	Service* service = (Service*)arg;
#ifdef SUPERSERVER
	ISC_STATUS *status = service->svc_status;
	*status++ = isc_arg_gds;
#endif

	TEXT name[MAXPATHLEN];
	gds__prefix(name, LOGFILE);
	FILE* file = fopen(name, "r");
	if (file != NULL) {
#ifdef SUPERSERVER
		*status++ = FB_SUCCESS;
		*status++ = isc_arg_end;
#endif
		service->svc_started();
		svc_started = true;
		TEXT buffer[100];
		while (!feof(file) && !ferror(file)) {
			fgets(buffer, sizeof(buffer), file);
#ifdef SUPERSERVER
			SVC_fprintf(service, "%s", buffer);
#else
			service_put(service, buffer, sizeof(buffer));
#endif
		}
	}

	if (!file || file && ferror(file)) {
#ifdef SUPERSERVER
		*status++ = isc_sys_request;
		if (!file) {
			SVC_STATUS_ARG(status, isc_arg_string, (void*)"fopen");
		}
		else {
			SVC_STATUS_ARG(status, isc_arg_string, (void*)"fgets");
		}
		*status++ = SYS_ARG;
		*status++ = errno;
		*status++ = isc_arg_end;
#endif
		if (!svc_started)
		{
			service->svc_started();
		}
	}

	if (file)
		fclose(file);

#ifdef SUPERSERVER
	SVC_finish(service, SVC_finished);
#else
	SVC_cleanup(service);
#endif
	return (FINI_OK);
}


static void get_options(const UCHAR*	spb,
						USHORT	spb_length,
						TEXT*	scratch,
						Serv_param_block*	options)
{
/**************************************
 *
 *	g e t _ o p t i o n s
 *
 **************************************
 *
 * Functional description
 *	Parse service parameter block picking up options and things.
 *
 **************************************/
	MOVE_CLEAR(options, (SLONG) sizeof(Serv_param_block));
	const UCHAR* p = spb;
	const UCHAR* const end_spb = p + spb_length;

	if (p < end_spb && (*p != isc_spb_version1 && *p != isc_spb_version))
		ERR_post(isc_bad_spb_form, isc_arg_gds, isc_wrospbver, 0);

	while (p < end_spb)
		switch (*p++) {
		case isc_spb_version1:
			options->spb_version = isc_spb_version1;
			break;

		case isc_spb_version:
			options->spb_version = *p++;
			break;

		case isc_spb_sys_user_name:
			options->spb_sys_user_name = get_string_parameter(&p, &scratch);
			break;

		case isc_spb_user_name:
			options->spb_user_name = get_string_parameter(&p, &scratch);
			break;

		case isc_spb_password:
			options->spb_password = get_string_parameter(&p, &scratch);
			break;

		case isc_spb_password_enc:
			options->spb_password_enc = get_string_parameter(&p, &scratch);
			break;

		case isc_spb_command_line:
			options->spb_command_line = get_string_parameter(&p, &scratch);
			break;

		default:
			{
				const USHORT l = *p++;
				p += l;
			}
		}
}


static TEXT* get_string_parameter(const UCHAR** spb_ptr, TEXT** opt_ptr)
{
/**************************************
 *
 *	g e t _ s t r i n g _ p a r a m e t e r
 *
 **************************************
 *
 * Functional description
 *	Pick up a string valued parameter, copy it to a running temp,
 *	and return pointer to copied string.
 *
 **************************************/
	TEXT* opt = *opt_ptr;
	const UCHAR* spb = *spb_ptr;

	USHORT l = *spb++;
	if (l)
		do {
			*opt++ = *spb++;
		} while (--l);

	*opt++ = 0;
	*spb_ptr = spb;
	spb = (UCHAR *) * opt_ptr;
	*opt_ptr = opt;

	return (TEXT *) spb;
}


#ifndef SUPERSERVER
static void io_error(
					 const TEXT* string,
					 SLONG status,
					 const TEXT* filename, ISC_STATUS operation,
					 bool reenter_flag)
{
/**************************************
 *
 *	i o _ e r r o r
 *
 **************************************
 *
 * Functional description
 *	Report an I/O error.  If the reenter_flag
 *	is true, re-enter the scheduler.
 *
 **************************************/

#ifdef MULTI_THREAD
	if (reenter_flag)
		THREAD_ENTER();
#endif

	ERR_post(isc_io_error, isc_arg_string, string, isc_arg_string, filename,
			 isc_arg_gds, operation, SYS_ERR, status, 0);
}
#endif

#ifdef WIN_NT
#ifndef SUPERSERVER
static void service_close(Service* service)
{
/**************************************
 *
 *	s e r v i c e _ c l o s e		( W I N _ N T )
 *
 **************************************
 *
 * Functional description
 *	Shutdown the connection to a service.
 *	Simply close the input and output pipes.
 *
 **************************************/
	CloseHandle((HANDLE) service->svc_input);
	CloseHandle((HANDLE) service->svc_output);
}


static void service_fork(TEXT* service_path, Service* service)
{
/**************************************
 *
 *	s e r v i c e _ f o r k		( W I N _ N T )
 *
 **************************************
 *
 * Functional description
 *	Startup a service.
 *
 **************************************/
	SECURITY_ATTRIBUTES attr;
	const bool windows_nt = ISC_is_WinNT();

/* Only Create the pipes on Windows NT.  There is a bug on Windows
   95 that prohibits these handles from being converted by the
   child process
*/
	HANDLE my_input, my_output, pipe_input, pipe_output, pipe_error;
	my_input = pipe_output = my_output = pipe_input = INVALID_HANDLE_VALUE;
	if (windows_nt) {
		/* Set up input and output pipes and make them the stdin, stdout,
		   and stderr of the forked process. */

		attr.nLength = sizeof(SECURITY_ATTRIBUTES);
		attr.bInheritHandle = TRUE;
		attr.lpSecurityDescriptor = NULL;

		if (!CreatePipe(&my_input, &pipe_output, &attr, 0) ||
			!CreatePipe(&pipe_input, &my_output, &attr, 0) ||
			!DuplicateHandle(GetCurrentProcess(), pipe_output,
							 GetCurrentProcess(), &pipe_error, 0, TRUE,
							 DUPLICATE_SAME_ACCESS))
		{
			const SLONG status = GetLastError();
			CloseHandle(my_input);
			CloseHandle(pipe_output);
			CloseHandle(my_output);
			CloseHandle(pipe_input);
			ERR_post(isc_sys_request, isc_arg_string,
					 (my_output != INVALID_HANDLE_VALUE) ?
					 	"CreatePipe" : "DuplicateHandle",
					 SYS_ERR, status, 0);
		}
	}
	else {
		/* Create a temporary file and get the OS handle to
		   the file created.  This handle will be used in subsequent
		   calls to the windows API functions for working with the files
		 */
		char tmpPath[MAXPATHLEN];

		GetTempPath(MAXPATHLEN, tmpPath);
		const char* fname = _tempnam(tmpPath, "ibsvc");
		int tmp =
			_open(fname, _O_RDWR | _O_CREAT | _O_TEMPORARY,
				  _S_IREAD | _S_IWRITE);
		my_input = (HANDLE) _get_osfhandle(tmp);

		fname = _tempnam(tmpPath, "ibsvc");
		tmp =
			_open(fname, _O_RDWR | _O_CREAT | _O_TEMPORARY,
				  _S_IREAD | _S_IWRITE);
		my_output = (HANDLE) _get_osfhandle(tmp);

		if (my_input == INVALID_HANDLE_VALUE ||
			my_output == INVALID_HANDLE_VALUE)
		{
			CloseHandle(my_input);
			CloseHandle(my_output);
			ERR_post(isc_sys_request, isc_arg_string, "CreateFile", SYS_ERR,
					 errno, 0);
		}
	}

/* Make sure we have buffers that are large enough to hold the number
   and size of the command line arguments.  Add some extra space for
   the pipe's file handles. */
   
	TEXT* p;

	USHORT len = strlen(service_path) + strlen(service->svc_switches) + 16;
	for (p = service->svc_switches; *p;)
		if (*p++ == ' ')
			len += 2;
			
	TEXT argv_data_buf[512];
	TEXT* argv_data;
	if (len > sizeof(argv_data_buf))
		argv_data = (TEXT*) gds__alloc((SLONG) len);
	else
		argv_data = argv_data_buf;
/* FREE: at procedure return */
	if (!argv_data)				/* NOMEM: */
		ERR_post(isc_virmemexh, 0);

/* Create a command line. */

	bool svc_flag = false; // Unless there's some ugly macro, this var seems useless

	const TEXT* q = service_path;
	for (p = argv_data; *p = *q++; p++);

	q = service->svc_switches;
	if (*q)
		*p++ = ' ';

	while (*q == ' ')
		q++;
	bool user_quote = false;
	while (*q) {
		TEXT* arg = p;
		*p++ = '\"';
		bool quote_flag = false;
		while (((*p = *q++) && *p != ' ') || user_quote) {
			if (*p == '\\' && *q == ' ' && !user_quote) {
				*p = ' ';
				q++;
				quote_flag = true;
			}
			if (*p == '"') {
				user_quote = !user_quote;
				p++;
				continue;
			}
			p++;
		}
		if (!quote_flag) {
			*arg = ' ';
			if (!strncmp(arg, " -svc", p - arg)) {
				if (windows_nt)
					sprintf(p, "_re %lu %lu %lu", (ULONG) pipe_input, (ULONG) pipe_output,
							(ULONG) pipe_error);
				else
					sprintf(p, "_re %lu %lu %lu", (ULONG) my_output, (ULONG) my_input,
							(ULONG) my_output);
				p += strlen(p);
				*p = q[-1];
				svc_flag = true;
			}
		}
		else {
			p[1] = p[0];
			*p++ = '\"';
		}
		if (!*p)
			break;
		*p++ = ' ';
		while (*q == ' ')
			q++;
	}

	for (p = argv_data; *p; p++) {
		if (*p == SVC_TRMNTR)
			*p = ' ';
	}

	THREAD_EXIT();

	STARTUPINFO start_crud;
	start_crud.cb = sizeof(STARTUPINFO);
	start_crud.lpReserved = NULL;
	start_crud.lpReserved2 = NULL;
	start_crud.cbReserved2 = 0;
	start_crud.lpDesktop = NULL;
	start_crud.lpTitle = NULL;
	start_crud.dwFlags = STARTF_USESHOWWINDOW;
	start_crud.wShowWindow = SW_HIDE;

	PROCESS_INFORMATION pi;
	const USHORT ret = CreateProcess(NULL,
							  argv_data,
							  NULL,
							  NULL,
							  TRUE,
							  NORMAL_PRIORITY_CLASS,
							  NULL,
							  NULL,
							  &start_crud, &pi);
	const SLONG status = ret ? 0 : GetLastError();

	if (windows_nt) {
		CloseHandle(pipe_input);
		CloseHandle(pipe_output);
		CloseHandle(pipe_error);
	}
	THREAD_ENTER();

	if (argv_data != argv_data_buf)
		gds__free(argv_data);

	if (!ret)
		ERR_post(isc_sys_request, isc_arg_string, "CreateProcess", SYS_ERR,
				 status, 0);

	DuplicateHandle(GetCurrentProcess(), pi.hProcess,
					GetCurrentProcess(), (PHANDLE) & service->svc_handle,
					0, TRUE, DUPLICATE_SAME_ACCESS);

	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);

	service->svc_input = (void *) my_input;
	service->svc_output = (void *) my_output;
}


static void service_get(Service* service,
						SCHAR * buffer,
						USHORT length,
						USHORT flags,
						USHORT timeout,
						USHORT * return_length)
{
/**************************************
 *
 *      s e r v i c e _ g e t           ( W I N _ N T )
 *
 **************************************
 *
 * Functional description
 *      Get input from a service.  It is assumed
 *      that we have checked out of the scheduler.
 *
 **************************************/
	SLONG n = 0L;
	SCHAR *buf = buffer;
/* Kludge for PeekNamedPipe to work on Win NT 3.5 */
	UCHAR temp_buf[600];
	DWORD temp_len;
	const bool windows_nt = ISC_is_WinNT();

	*return_length = 0;
	service->svc_flags &= ~SVC_timeout;
	is_service_running(service);

	if (timeout) {
		/* If a timeout period was given, check every .1 seconds to see if
		   input is available from the pipe.  When something shows up, read
		   what's available until all data has been read, or timeout occurs.
		   Otherwise, set the timeout flag and return.
		   Fall out of the loop if a BROKEN_PIPE error occurs.
		 */
		SSHORT iter = timeout * 10; // nice signed/unsigned assignment. :-)
		while ((iter--) && ((buf - buffer) < length)) {
			/* PeekNamedPipe sometimes return wrong &n, need to get real
			   length from &temp_len until it works */
			if (windows_nt
				&& !PeekNamedPipe((HANDLE) service->svc_input, temp_buf, 600,
								  &temp_len, (ULONG*) &n, NULL))
				{
				if (GetLastError() == ERROR_BROKEN_PIPE) {
					service->svc_flags |= SVC_finished;
					break;
				}
				io_error("PeekNamedPipe", GetLastError(), "service pipe",
						 isc_io_read_err, true);
			}
			else {
				DWORD dwCurrentFilePosition;
				/* Set the file pointer to the beginning of the file if we are at the end of the
				   file. */
				temp_len = GetFileSize(service->svc_input, NULL);
				dwCurrentFilePosition =
					SetFilePointer(service->svc_input, 0, NULL, FILE_CURRENT);

				if (temp_len && temp_len == dwCurrentFilePosition)
					SetFilePointer(service->svc_input, 0, NULL, FILE_BEGIN);
			}

			if (temp_len) {
				/* If data is available, read as much as will fit in buffer */
				temp_len = MIN(temp_len, (DWORD) (length - (buf - buffer)));
				const USHORT bytes_read =
					service_read(service, buf, (USHORT) temp_len, flags);
				buf += bytes_read;

				if (bytes_read < temp_len
					|| service->svc_flags & SVC_finished) 
				{
					/* We stopped w/out reading full length, must have hit
					   a newline or special character. */
					break;
				}
			}
			else {
				/* PeekNamedPipe() is not returning ERROR_BROKEN_PIPE in WIN95. So,
				   we are going to use WaitForSingleObject() to check if the process
				   on the other end of the pipe is still active. */
				if (!windows_nt &&
					WaitForSingleObject((HANDLE) service->svc_handle,
										1) != WAIT_TIMEOUT)
				{
					service->svc_flags |= SVC_finished;
					break;
				}

				/* No data, so wait a while */
				Sleep(100);
			}
		}
		/* If we timed out, set the appropriate flags */
		if (iter < 0 && !(service->svc_flags & SVC_finished))
			service->svc_flags |= SVC_timeout;
	}
	else {
		buf += service_read(service, buf, length, flags);
	}

	*return_length = buf - buffer;
}


static void service_put(Service* service, const SCHAR* buffer, USHORT length)
{
/**************************************
 *
 *	s e r v i c e _ p u t		( W I N _ N T )
 *
 **************************************
 *
 * Functional description
 *	Send output to a service.  It is assumed
 *	that we have checked out of the scheduler.
 *
 **************************************/
	SLONG n;

	is_service_running(service);

	while (length) {
		if (!WriteFile((HANDLE) service->svc_output, buffer, (SLONG) length,
					   (ULONG*) &n, NULL))
		{
			io_error("WriteFile", GetLastError(), "service pipe",
					 isc_io_write_err, true);
		}
		length -= (USHORT) n;
		buffer += n;
	}

	if (!FlushFileBuffers((HANDLE) service->svc_output))
		io_error("FlushFileBuffers", GetLastError(), "service pipe",
				 isc_io_write_err, true);
}


static USHORT service_read(Service* service, SCHAR * buffer, USHORT length, USHORT flags)
{
/**************************************
 *
 *      s e r v i c e _ r e a d         ( W I N _ N T )
 *
 **************************************
 *
 * Functional description
 *      Read data from the named pipe.
 *      Returns true if there is more data to be
 *      read, false if we found the newline or
 *      special character.
 *
 **************************************/
	SCHAR* buf = buffer;

	while (length) {
		SLONG n = 0;
		const SLONG len = (flags & GET_BINARY) ? length : 1;
		if (ReadFile((HANDLE) service->svc_input, buf, len, (ULONG*) &n, NULL) ||
			GetLastError() == ERROR_BROKEN_PIPE)
		{
			if (!n) {
				service->svc_flags |= SVC_finished;
				break;
			}
			length -= (USHORT) n;
			buf += n;
			if (((flags & GET_LINE) && buf[-1] == '\n') ||
				(!(flags & GET_BINARY) && buf[-1] == '\001'))
			{
				break;
			}
		}
		else
			io_error("ReadFile", GetLastError(), "service pipe",
					 isc_io_read_err, true);
	}

	if ((flags & GET_LINE) && buf[-1] == '\n')
		buf[-1] = ' ';

	return buf - buffer;
}
#endif /* ifndef SUPERSERVER */
#endif /* WIN_NT */


#ifdef SUPERSERVER
static USHORT service_add_one(USHORT i)
{
/**************************************
 *
 *	s e r v i c e _ a d d _ o n e
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	return ((i % SVC_STDOUT_BUFFER_SIZE) + 1);
}


static USHORT service_empty(Service* service)
{
/**************************************
 *
 *	s e r v i c e _ e m p t y
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	if (service_add_one(service->svc_stdout_tail) == service->svc_stdout_head)
		return 1;

	return 0;
}


static USHORT service_full(Service* service)
{
/**************************************
 *
 *	s e r v i c e _ f u l l
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	if (service_add_one(service_add_one(service->svc_stdout_tail)) ==
		service->svc_stdout_head)
	{
		return (1);
	}
	else
		return (0);
}


static UCHAR service_dequeue_byte(Service* service)
{
/**************************************
 *
 *	s e r v i c e _ d e q u e u e _ b y t e
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	const UCHAR ch = service->svc_stdout[service->svc_stdout_head];
	service->svc_stdout_head = service_add_one(service->svc_stdout_head);

	return (ch);
}


static void service_enqueue_byte(UCHAR ch, Service* service)
{
/**************************************
 *
 *	s e r v i c e _ e n q u e u e _ b y t e
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
/* Wait for space in buffer while service is not detached. */
	while (service_full(service) && !(service->svc_flags & SVC_detached))
		THREAD_SLEEP(1);

/* Ensure that service is not detached. */
	if (!(service->svc_flags & SVC_detached)) {
		service->svc_stdout_tail = service_add_one(service->svc_stdout_tail);
		service->svc_stdout[service->svc_stdout_tail] = ch;
	}
}


static void service_fork(ThreadEntryPoint* service_executable, Service* service)
{
/**************************************
 *
 *	s e r v i c e _ f o r k
 *
 **************************************
 *
 * Functional description
 *	Startup a service.
 *
 **************************************/
	TEXT* p;
	
	USHORT argc = 2;
	for (p = service->svc_switches; *p;)
		if (*p++ == ' ')
			argc++;

	service->svc_argc = argc;

	TEXT** arg = (TEXT **) gds__alloc((SLONG) ((argc + 1) * sizeof(TEXT *)));
	service->svc_argv = arg;
/* FREE: at SVC_detach() - Possible memory leak if ERR_post() occurs */
	if (!arg)					/* NOMEM: */
		ERR_post(isc_virmemexh, 0);

	*arg++ = (TEXT *)(service_executable);

/* Break up the command line into individual arguments. */

	const TEXT* q = p = service->svc_switches;

	while (*q == ' ')
		q++;
	while (*q) {
		*arg++ = p;
		while ((*p = *q++) && *p != ' ') {
			if (*p == '\\' && *q == ' ') {
				*p = ' ';
				q++;
			}
			p++;
		}
		if (!*p)
			break;
		*p++ = 0;
		while (*q == ' ')
			q++;
	}
	*arg = NULL;

	service->svc_stdout = (UCHAR*)gds__alloc((SLONG) SVC_STDOUT_BUFFER_SIZE + 1);
/* FREE: at SVC_detach() */
	if (!service->svc_stdout)	/* NOMEM: */
		ERR_post(isc_virmemexh, 0);

	THREAD_EXIT();
	gds__thread_start(service_executable,
						service, THREAD_medium, 0, (void*)&service->svc_handle);
	THREAD_ENTER();
}


static void service_get(Service*		service,
						SCHAR*	buffer,
						USHORT	length,
						USHORT	flags,
						USHORT	timeout,
						USHORT*	return_length)
{
/**************************************
 *
 *	s e r v i c e _ g e t
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

#ifdef HAVE_GETTIMEOFDAY
	struct timeval start_time, end_time;
	GETTIMEOFDAY(&start_time);
#else
	time_t start_time, end_time;
	time(&start_time);
#endif

	*return_length = 0;
	service->svc_flags &= ~SVC_timeout;

	while (length) {
		if (service_empty(service))
			THREAD_SLEEP(1);
#ifdef HAVE_GETTIMEOFDAY
		GETTIMEOFDAY(&end_time);
		const time_t elapsed_time = end_time.tv_sec - start_time.tv_sec;
#else
		time(&end_time);
		const time_t elapsed_time = end_time - start_time;
#endif
		if ((timeout) && (elapsed_time >= timeout)) {
			service->svc_flags &= SVC_timeout;
			return;
		}

		while (!service_empty(service) && length > 0) {
			const int ch = service_dequeue_byte(service);
			length--;

			/* If returning a line of information, replace all new line
			 * characters with a space.  This will ensure that the output is
			 * consistent when returning a line or to eof
			 */
			if ((flags & GET_LINE) && (ch == '\n')) {
				buffer[(*return_length)++] = ' ';
				return;
			}
			else
				buffer[(*return_length)++] = ch;
		}

		if (service_empty(service) && (service->svc_flags & SVC_finished))
			return;
	}
}


static void service_put(Service* service, const SCHAR* buffer, USHORT length)
{
/**************************************
 *
 *	s e r v i c e _ p u t
 *
 **************************************
 *
 * Functional description
 *	Send output to a service.  It is assumed
 *	that we have checked out of the scheduler.
 *
 **************************************/

/* Nothing */
}
#endif /* SUPERSERVER */


#if !defined(WIN_NT) && !defined(SUPERSERVER)
static void service_close(Service* service)
{
/**************************************
 *
 *	s e r v i c e _ c l o s e		( G E N E R I C )
 *
 **************************************
 *
 * Functional description
 *	Shutdown the connection to a service.
 *	Simply close the input and output pipes.
 *
 **************************************/

	fclose((FILE *) service->svc_input);
	fclose((FILE *) service->svc_output);
}


static void service_fork(TEXT* service_path, Service* service)
{
/**************************************
 *
 *	s e r v i c e _ f o r k		( G E N E R I C )
 *
 **************************************
 *
 * Functional description
 *	Startup a service.
 *
 **************************************/
	int pair1[2], pair2[2];

	if (pipe(pair1) < 0 || pipe(pair2) < 0)
		io_error("pipe", errno, "", isc_io_create_err, false);

/* Probe service executable to see it if plausibly exists. */

	struct stat stat_buf;
	if (statistics(service_path, &stat_buf) == -1)
		io_error("stat", errno, service_path, isc_io_access_err, false);

/* Make sure we have buffers that are large enough to hold the number
   and size of the command line arguments. */

	TEXT* p;
	
	USHORT argc = 2;
	for (p = service->svc_switches; *p;)
	{
		if (*p == ' ')
		{
			argc++;
			while (*p == ' ')
				p++;
		}
		else
		{
			if (*p == SVC_TRMNTR)
			{
				while (*p++ && *p != SVC_TRMNTR);
				fb_assert (*p == SVC_TRMNTR);
			}
			p++;
		}
	}

	// Hardcoded, ISC_STATUS_LEN, platform specific or what????
	TEXT* argv_buf[20];
	TEXT** argv;
	if (argc > FB_NELEM(argv_buf))
		argv = (TEXT **) gds__alloc((SLONG) (argc * sizeof(TEXT *)));
	else
		argv = argv_buf;
/* FREE: at procedure return */
	if (!argv)					/* NOMEM: */
		ERR_post(isc_virmemexh, 0);
	service->svc_argc = argc;

	TEXT argv_data_buf[512];
	TEXT* argv_data;
	const USHORT len = strlen(service->svc_switches) + 1;
	if (len > sizeof(argv_data_buf))
		argv_data = (TEXT *) gds__alloc((SLONG) len);
	else
		argv_data = argv_data_buf;
/* FREE: at procedure return */
	if (!argv_data) {			/* NOMEM: */
		if (argv != argv_buf)
			gds__free(argv);
		ERR_post(isc_virmemexh, 0);
	}

/* Break up the command line into individual arguments. */

	TEXT** arg = argv;
	*arg++ = service_path;

	p = argv_data;
	const TEXT* q = service->svc_switches;

	while (*q == ' ')
		q++;
	while (*q)
	{
		*arg = p;
		while (*p = *q++)
		{
			if (*p == ' ') break;

			if (*p == SVC_TRMNTR)
			{
				*arg = ++p;	/* skip terminator */
				while (*p = *q++)
						/* If *q points to the last argument, then terminate the argument */
					if ((*q == 0 || *q == ' ') && *p == SVC_TRMNTR)
					{
						*p = '\0';
						break;
					}
					else
						p++;
			}

			if (*p == '\\' && *q == ' ')
			{
				*p = ' ';
				q++;
			}
			p++;
		}
		arg++;
		if (!*p)
			break;
		*p++ = '\0';
		while (*q == ' ')
			q++;
	}
	*arg = NULL;

/* At last we can fork the sub-process.  If the fork succeeds, repeat
   it so that we don't have defunct processes hanging around. */

	THREAD_EXIT();

	int pid;
	
	switch (pid = vfork()) {
	case -1:
		THREAD_ENTER();
		if (argv != argv_buf)
			gds__free(argv);
		if (argv_data != argv_data_buf)
			gds__free(argv_data);
		ERR_post(isc_sys_request, isc_arg_string, "vfork", SYS_ERR, errno, 0);
		break;

	case 0:
		if (vfork() != 0)
			_exit(FINI_OK);

		close(pair1[0]);
		close(pair2[1]);
		if (pair2[0] != 0) {
			close(0);
			dup(pair2[0]);
			close(pair2[0]);
		}
		if (pair1[1] != 1) {
			close(1);
			dup(pair1[1]);
			close(pair1[1]);
		}
		close(2);
		dup(1);
#ifdef DEV_BUILD
		{
			char buf[2 * MAXPATHLEN];
			char** s = argv;

			strcpy (buf, "service_fork:");
			while (*s != (char *)0)
			{
				strcat(buf, " ");
				strcat(buf, *s);
				s++;
			}
			gds__log(buf);
		}
#endif
		execvp(argv[0], argv);
		_exit(FINI_ERROR);
	}

	close(pair1[1]);
	close(pair2[0]);

	waitpid(pid, NULL, 0);

	THREAD_ENTER();

	if (argv != argv_buf)
		gds__free(argv);
	if (argv_data != argv_data_buf)
		gds__free(argv_data);

	if (!(service->svc_input = (void *) fdopen(pair1[0], "r")) ||
		!(service->svc_output = (void *) fdopen(pair2[1], "w")))
	{
		io_error("fdopen", errno, "service path", isc_io_access_err,
				 false);
	}
}


static void service_get(
						Service* service,
						SCHAR * buffer,
						USHORT length,
						USHORT flags, USHORT timeout, USHORT * return_length)
{
/**************************************
 *
 *	s e r v i c e _ g e t		( G E N E R I C )
 *
 **************************************
 *
 * Functional description
 *	Get input from a service.  It is assumed
 *	that we have checked out of the scheduler.
 *
 **************************************/
	struct itimerval sv_timr;
	struct sigaction sv_hndlr;

	is_service_running(service);

	errno = 0;
	service->svc_flags &= ~SVC_timeout;
	SCHAR* buf = buffer;

	SSHORT iter = 0;
	if (timeout) {
		ISC_set_timer((SLONG) (timeout * 100000), timeout_handler, service,
					  (SLONG*)&sv_timr, (void**)&sv_hndlr);
		iter = timeout * 10;
	}

	while (!timeout || iter) {
		const int c = getc((FILE *) service->svc_input);
		if (c != EOF) {
			*buf++ = c;
			if (!(--length))
				break;
			if (((flags & GET_LINE) && c == '\n') ||
				(!(flags & GET_BINARY) && c == '\001'))
			{
				break;
			}
		}
		else if (!errno) {
			service->svc_flags |= SVC_finished;
			break;
		}
		else if (SYSCALL_INTERRUPTED(errno)) {
			if (timeout)
				--iter;
		}
		else {
			const int errno_save = errno;
			if (timeout)
				ISC_reset_timer(timeout_handler, service, (SLONG*)&sv_timr,
								(void**)&sv_hndlr);
			io_error("getc", errno_save, "service pipe", isc_io_read_err,
					 true);
		}
	}

	if (timeout) {
		ISC_reset_timer(timeout_handler, service, (SLONG*)&sv_timr, (void**)&sv_hndlr);
		if (!iter)
			service->svc_flags |= SVC_timeout;
	}

	*return_length = buf - buffer;
}


static void service_put(Service* service, const SCHAR* buffer, USHORT length)
{
/**************************************
 *
 *	s e r v i c e _ p u t		( G E N E R I C )
 *
 **************************************
 *
 * Functional description
 *	Send output to a service.  It is assumed
 *	that we have checked out of the scheduler.
 *
 **************************************/

	is_service_running(service);

	while (length--) {
		if (putc(*buffer, (FILE *) service->svc_output) != EOF)
			buffer++;
		else if (SYSCALL_INTERRUPTED(errno)) {
			rewind((FILE *) service->svc_output);
			length++;
		}
		else
			io_error("putc", errno, "service pipe", isc_io_write_err,
					 true);
	}

	if (fflush((FILE *) service->svc_output) == EOF)
		io_error("fflush", errno, "service pipe", isc_io_write_err, true);
}


static void timeout_handler(void* service)
{
/**************************************
 *
 *	t i m e o u t _ h a n d l e r
 *
 **************************************
 *
 * Functional description
 *	Called when no input has been received
 *	through a pipe for a specificed period
 *	of time.
 *
 **************************************/
}
#endif // !defined(WIN_NT) && !defined(SUPERSERVER)


void SVC_cleanup(Service* service)
{
/**************************************
 *
 *	S V C _ c l e a n u p
 *
 **************************************
 *
 * Functional description
 *	Cleanup memory used by service.
 *
 **************************************/

#ifndef SUPERSERVER
/* If we forked an executable, close down it's pipes */

	if (service->svc_flags & SVC_forked)
		service_close(service);
#else

	if (service->svc_stdout) {
		gds__free(service->svc_stdout);
		service->svc_stdout = NULL;
	}
	if (service->svc_argv) {
		gds__free(service->svc_argv);
		service->svc_argv = NULL;
	}
#endif

	if (service->svc_resp_buf)
		gds__free(service->svc_resp_buf);

	if (service->svc_switches != NULL)
		gds__free(service->svc_switches);

	if (service->svc_status != NULL)
		gds__free(service->svc_status);

#ifdef WIN_NT
	CloseHandle((HANDLE) service->svc_handle);
#endif

//	gds__free(service);
	delete service;
}


void SVC_finish(Service* service, USHORT flag)
{
/**************************************
 *
 *	S V C _ f i n i s h
 *
 **************************************
 *
 * Functional description
 *	o Set the flag (either SVC_finished for the main service thread or
 *	  SVC_detached for the client thread).
 *	o If both main thread and client thread are completed that is main
 *	  thread is finished and client is detached then free memory
 *	  used by service.
 *
 **************************************/

	THD_MUTEX_LOCK(svc_mutex);
	if (service && ((flag == SVC_finished) || (flag == SVC_detached)))
	{
		service->svc_flags |= flag;
		if ((service->svc_flags & SVC_finished) &&
			(service->svc_flags & SVC_detached))
		{
			SVC_cleanup(service);
		}
		else if (service->svc_flags & SVC_finished)
		{
			if (service->svc_service && service->svc_service->serv_in_use)
			{
				*(service->svc_service->serv_in_use) = false;
			}

			service->svc_flags &= ~SVC_thd_running;

#ifdef WIN_NT
			CloseHandle((HANDLE) service->svc_handle);
#endif
			service->svc_handle = 0;
		}
	}
	THD_MUTEX_UNLOCK(svc_mutex);
}


static void conv_switches(
						  USHORT spb_length,
						  USHORT opt_switch_len,
						  const SCHAR* spb, TEXT** switches)
{
/**************************************
 *
 *	c o n v _ s w i t c h e s
 *
 **************************************
 *
 * Functional description
 *	Convert spb flags to utility switches.
 *
 **************************************/
	const TEXT* p = spb;

	if (*p < isc_action_min || *p > isc_action_max)
		return;					/* error action not defined */

/* Calculate the total length */
	const USHORT total = process_switches(spb_length, p, NULL);
	if (total == 0)
		return;

	*switches =
		(TEXT *) gds__alloc(total + opt_switch_len +
							sizeof(SERVICE_THD_PARAM) + 1);
/* NOMEM: return error */
	if (!*switches)
		ERR_post(isc_virmemexh, 0);

	strcpy(*switches, SERVICE_THD_PARAM);
	strcat(*switches, " ");
	process_switches(spb_length, p,
					 *switches + strlen(SERVICE_THD_PARAM) + 1);
	return;
}


static const TEXT* find_switch(int in_spb_sw, const in_sw_tab_t* table)
{
/**************************************
 *
 *      f i n d _ s w i t c h
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	for (const in_sw_tab_t* in_sw_tab = table; in_sw_tab->in_sw_name; in_sw_tab++)
	{
		if (in_spb_sw == in_sw_tab->in_spb_sw)
			return in_sw_tab->in_sw_name;
	}

	return NULL;
}


static USHORT process_switches(
							   USHORT spb_length,
							   const SCHAR* spb, TEXT* switches)
{
/**************************************
 *
 *      p r o c e s s _ s w i t c h e s
 *
 **************************************
 *
 *Functional description
 *   Loop through the appropriate switch table
 *   looking for the text for the given command switch.
 *
 *   Calling this function with switches = NULL returns
 *   the number of bytes to allocate for the switches and
 *   parameters.
 *
 **************************************/
	if (spb_length == 0)
		return 0;

	const TEXT* p = spb;
	USHORT len = spb_length;
	TEXT* sw = switches;

	// CVC: Isn't ISC_USHORT an external name?
	const ISC_USHORT svc_action = *p;

	if (len > 1) {
		++p;
		--len;
	}

	USHORT total = 0;				// total length of the command line
	bool found = false;

	while (len > 0) {
		switch (svc_action) {
		case isc_action_svc_delete_user:
		case isc_action_svc_display_user:
			if (!found) {
				if (spb != p) {
					--p;
					++len;
				}
				fb_assert(spb == p);
				if (!get_action_svc_parameter(&p, gsec_action_in_sw_table,
											  &sw, &total, &len))
				{
					return 0;
				}
				else {
					found = true;
					/* in case of "display all users" the spb buffer contains
					   nothing but isc_action_svc_display_user */
					if (len == 0)
						break;
				}
			}

			switch (*p) {
			case isc_spb_sec_username:
				++p;
				--len;
				get_action_svc_string(&p, &sw, &total, &len);
				break;

			default:
				return 0;
				break;
			}
			break;

		case isc_action_svc_add_user:
		case isc_action_svc_modify_user:
			if (!found) {
				if (spb != p) {
					--p;
					++len;
				}
				fb_assert(spb == p);
				if (!get_action_svc_parameter(&p, gsec_action_in_sw_table,
											  &sw, &total, &len))
				{
					return 0;
				}
				else {
					found = true;
					if (*p != isc_spb_sec_username) {
						/* unexpected service parameter block:
						   expected %d, encountered %d */
						ERR_post(isc_unexp_spb_form, isc_arg_string,
								 SVC_err_string(SPB_SEC_USERNAME,
												strlen(SPB_SEC_USERNAME)),
								 0);
					}
				}
			}

			switch (*p) {
			case isc_spb_sec_userid:
			case isc_spb_sec_groupid:
				if (!get_action_svc_parameter(&p, gsec_in_sw_table,
											  &sw, &total, &len))
				{
					return 0;
				}
				get_action_svc_data(&p, &sw, &total, &len);
				break;

			case isc_spb_sec_username:
				++p;
				--len;
				get_action_svc_string(&p, &sw, &total, &len);
				break;
			case isc_spb_sql_role_name:
			case isc_spb_sec_password:
			case isc_spb_sec_groupname:
			case isc_spb_sec_firstname:
			case isc_spb_sec_middlename:
			case isc_spb_sec_lastname:
				if (!get_action_svc_parameter(&p, gsec_in_sw_table,
											  &sw, &total, &len))
				{
					return 0;
				}
				get_action_svc_string(&p, &sw, &total, &len);
				break;

			default:
				return 0;
				break;
			}
			break;

		case isc_action_svc_db_stats:
			switch (*p) {
			case isc_spb_dbname:
				++p;
				--len;
				get_action_svc_string(&p, &sw, &total, &len);
				break;

			case isc_spb_options:
				++p;
				--len;
				if (!get_action_svc_bitmask(&p, dba_in_sw_table,
											&sw, &total, &len))
				{
					return 0;
				}
				break;

			case isc_spb_command_line: 
				++p;
				--len;
				get_action_svc_string(&p, &sw, &total, &len);
				break;

			default:
				return 0;
				break;
			}
			break;

		case isc_action_svc_backup:
		case isc_action_svc_restore:
			switch (*p) {
			case isc_spb_bkp_file:
			case isc_spb_dbname:
			case isc_spb_sql_role_name:
				++p;
				--len;
				get_action_svc_string(&p, &sw, &total, &len);
				break;
			case isc_spb_options:
				++p;
				--len;
				if (!get_action_svc_bitmask(&p, burp_in_sw_table,
											&sw, &total, &len))
				{
					return 0;
				}
				break;
			case isc_spb_bkp_length:
			case isc_spb_res_length:
				++p;
				--len;
				get_action_svc_data(&p, &sw, &total, &len);
				break;
			case isc_spb_bkp_factor:
			case isc_spb_res_buffers:
			case isc_spb_res_page_size:
				if (!get_action_svc_parameter(&p, burp_in_sw_table,
											  &sw, &total, &len))
				{
					return 0;
				}
				get_action_svc_data(&p, &sw, &total, &len);
				break;
			case isc_spb_res_access_mode:
				++p;
				--len;
				if (!get_action_svc_parameter(&p, burp_in_sw_table,
											  &sw, &total, &len))
				{
					return 0;
				}
				break;
			case isc_spb_verbose:
				if (!get_action_svc_parameter(&p, burp_in_sw_table,
											  &sw, &total, &len))
				{
					return 0;
				}
				break;
			default:
				return 0;
				break;
			}
			break;

		case isc_action_svc_repair:
		case isc_action_svc_properties:
			switch (*p) {
			case isc_spb_dbname:
				++p;
				--len;
				get_action_svc_string(&p, &sw, &total, &len);
				break;
			case isc_spb_options:
				++p;
				--len;
				if (!get_action_svc_bitmask(&p, alice_in_sw_table,
											&sw, &total, &len))
				{
					return 0;
				}
				break;
			case isc_spb_prp_page_buffers:
			case isc_spb_prp_sweep_interval:
			case isc_spb_prp_shutdown_db:
			case isc_spb_prp_deny_new_attachments:
			case isc_spb_prp_deny_new_transactions:
			case isc_spb_prp_set_sql_dialect:
			case isc_spb_rpr_commit_trans:
			case isc_spb_rpr_rollback_trans:
			case isc_spb_rpr_recover_two_phase:
				if (!get_action_svc_parameter(&p, alice_in_sw_table,
											  &sw, &total, &len))
				{
					return 0;
				}
				get_action_svc_data(&p, &sw, &total, &len);
				break;
			case isc_spb_prp_write_mode:
			case isc_spb_prp_access_mode:
			case isc_spb_prp_reserve_space:
				++p;
				--len;
				if (!get_action_svc_parameter(&p, alice_in_sw_table,
											  &sw, &total, &len))
				{
					return 0;
				}
				break;
			default:
				return 0;
				break;
			}
			break;
		default:
			return 0;
			break;
		}
	}

	if (sw && *(sw - 1) == ' ')
		*--sw = '\0';

	return total;
}


static bool get_action_svc_bitmask(
									  const TEXT** spb,
									  const in_sw_tab_t* table,
									  TEXT** cmd,
									  USHORT* total, USHORT* len)
{
/**************************************
 *
 *	g e t _ a c t i o n _ s v c _ b i t m a s k
 *
 **************************************
 *
 * Functional description
 *	Get bitmask from within spb buffer,
 *	find corresponding switches within specified table,
 *	add them to the command line,
 *	adjust pointers.
 *
 **************************************/
	const ISC_ULONG opt =
		gds__vax_integer(reinterpret_cast<const UCHAR*>(*spb),
						 sizeof(ISC_ULONG));
	ISC_ULONG mask = 1;
	for (int count = (sizeof(ISC_ULONG) * 8) - 1; count; --count) {
		if (opt & mask) {
			const TEXT* s_ptr = find_switch((opt & mask), table);
			if (!s_ptr)
				return false;
			else {
				if (*cmd) {
					sprintf(*cmd, "-%s ", s_ptr);
					*cmd += 1 + strlen(s_ptr) + 1;
				}
				*total += 1 + strlen(s_ptr) + 1;
			}
		}
		mask = mask << 1;
	}

	*spb += sizeof(ISC_ULONG);
	*len -= sizeof(ISC_ULONG);
	return true;
}


static void get_action_svc_string(
								  const TEXT** spb,
								  TEXT** cmd, USHORT* total, USHORT* len)
{
/**************************************
 *
 *      g e t _ a c t i o n _ s v c _ s t r i n g
 *
 **************************************
 *
 * Functional description
 *      Get string from within spb buffer,
 *      add it to the command line, adjust pointers.
 *
 *      All string parameters are delimited by SVC_TRMNTR.  This
 *      is done to ensure that paths with spaces are handled correctly
 *      when creating the argc / argv paramters for the service.
 *
 **************************************/
	const ISC_USHORT l = gds__vax_integer(reinterpret_cast<const UCHAR*>(*spb),
						 sizeof(ISC_USHORT));

/* Do not go beyond the bounds of the spb buffer */
	if (l > *len)
		ERR_post(isc_bad_spb_form, 0);

	*spb += sizeof(ISC_USHORT);
	if (*cmd)
	{
		**cmd = SVC_TRMNTR;
		*cmd += 1;
		MOVE_FASTER(*spb, *cmd, l);
		*cmd += l;
		**cmd = SVC_TRMNTR;
		*cmd += 1;
		**cmd = ' ';
		*cmd += 1;
	}
	*spb += l;
	*total += l + 1 + 2;		/* Two SVC_TRMNTR for strings */
	*len -= sizeof(ISC_USHORT) + l;
}


static void get_action_svc_data(
								const TEXT** spb,
								TEXT** cmd, USHORT* total, USHORT* len)
{
/**************************************
 *
 *      g e t _ a c t i o n _ s v c _ d a t a
 *
 **************************************
 *
 * Functional description
 *      Get data from within spb buffer,
 *      add it to the command line, adjust pointers.
 *
 **************************************/
	TEXT buf[64];

	const ISC_ULONG ll =
		gds__vax_integer(reinterpret_cast<const UCHAR*>(*spb),
						 sizeof(ISC_ULONG));
	sprintf(buf, "%"ULONGFORMAT" ", ll);
	if (*cmd) {
		sprintf(*cmd, "%lu ", ll);
		*cmd += strlen(buf);
	}
	*spb += sizeof(ISC_ULONG);
	*total += strlen(buf) + 1;
	*len -= sizeof(ISC_ULONG);
}


static bool get_action_svc_parameter(
										const TEXT** spb,
										const in_sw_tab_t* table,
										TEXT** cmd,
										USHORT* total, USHORT* len)
{
/**************************************
 *
 *      g e t _ a c t i o n _ s v c _ p a r a m e t e r
 *
 **************************************
 *
 * Functional description
 *      Get parameter from within spb buffer,
 *      find corresponding switch within specified table,
 *	add it to the command line,
 *	adjust pointers.
 *
 **************************************/
	const TEXT* s_ptr = find_switch(**spb, table);
	if (!s_ptr)
		return false;

	if (*cmd) {
		sprintf(*cmd, "-%s ", s_ptr);
		*cmd += 1 + strlen(s_ptr) + 1;
	}

	*spb += 1;
	*total += 1 + strlen(s_ptr) + 1;
	*len -= 1;
	return true;
}

#ifdef DEBUG
/* The following two functions are temporary stubs and will be
 * removed as the services API takes shape.  They are used to
 * test that the paths for starting services and parsing command-lines
 * are followed correctly.
 */
THREAD_ENTRY_DECLARE test_thread(THREAD_ENTRY_PARAM)
{
	gds__log("Starting service");
	return FINI_OK;
}

void test_cmd(USHORT spb_length, SCHAR* spb, TEXT** switches)
{
	gds__log("test_cmd called");
}
#endif

