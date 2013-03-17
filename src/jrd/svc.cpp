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
//#include "../common/classes/timestamp.h"
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
#include "../common/classes/ClumpletWriter.h"
#include "../jrd/ibase.h"
#include "../common/utils_proto.h"
#include "../jrd/scl.h"
#include "../common/config/config.h"

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

namespace Jrd {
	Service::Service(serv_entry *se, Firebird::MemoryPool& p) :
		svc_parsed_sw(p), 
		svc_handle(0), svc_status(svc_status_array), svc_input(0), svc_output(0),
		svc_stdout_head(0), svc_stdout_tail(0), svc_stdout(0), svc_argv(p), svc_argc(0),
		svc_service(se), svc_resp_buf(0), svc_resp_ptr(0), svc_resp_buf_len(0),
		svc_resp_len(0), svc_flags(0), svc_user_flag(0), svc_spb_version(0), svc_do_shutdown(false),
		svc_uses_security_database(false), svc_username(p), svc_enc_password(p), 
#ifdef TRUSTED_SERVICES
		svc_trusted_login(p),
#endif
		svc_switches(p), svc_perm_sw(p)

	{
		memset(svc_status_array, 0, sizeof svc_status_array);
	}
	
	Service::~Service()
	{
		// If we forked an executable, close down it's pipes
#ifndef SERVICE_THREAD
		if (svc_flags & SVC_forked)
		{
#ifndef WIN_NT
			if (svc_input)
			{
				fclose(svc_input);
			}
			if (svc_output)
			{
				fclose(svc_output);
			}
#endif //WIN_NT
		}
#else //SERVICE_THREAD
		if (svc_stdout)
		{
			gds__free(svc_stdout);
		}
#endif //SERVICE_THREAD

		if (svc_resp_buf)
		{
			gds__free(svc_resp_buf);
		}

#ifdef WIN_NT
		CloseHandle((HANDLE) svc_handle);
#endif
	}
	
	void Service::parseSwitches()
	{
		svc_parsed_sw = svc_switches;
		svc_parsed_sw.trim();
		svc_argc = 2;
		
		if (svc_parsed_sw.isEmpty())
		{
			svc_argv.getBuffer(svc_argc + 1)[1] = 0;
			return;
		}
		
		bool inStr = false;
		for (size_t i = 0; i < svc_parsed_sw.length(); ++i)
		{

			switch (svc_parsed_sw[i])
			{
			case SVC_TRMNTR:
				svc_parsed_sw.erase(i, 1);
				if (inStr)
				{
					if (i < svc_parsed_sw.length() && svc_parsed_sw[i] != SVC_TRMNTR)
					{
						inStr = false;
						--i;
					}
				}
				else
				{
					inStr = true;
					--i;
				}
				break;
				
			case ' ':
				if (!inStr)
				{
					++svc_argc;
					svc_parsed_sw[i] = 0;
				}
				break;
			}
		}
		
		const char** argv = svc_argv.getBuffer(svc_argc + 1);
		argv++;	// leave space for argv[0]
		*argv++ = svc_parsed_sw.c_str();
		for (const char *p = svc_parsed_sw.begin(); 
			p < svc_parsed_sw.end(); ++p)
		{
			if (!*p) 
			{
				*argv++ = p + 1;
			}
		}
		*argv = 0;
	}
} //namespace
	
using namespace Jrd;

#ifdef SERVICE_THREAD

void Service::svc_started()
{
	if (!(svc_flags & SVC_evnt_fired)) {
		svc_flags |= SVC_evnt_fired;
		svcStart.release();
	}
}

#endif /* SERVICE_THREAD */

/* This checks if the service has forked a process.  If not,
   it will post the isc_svcnoexe error. */

static inline void is_service_running(const Service* service)
{
	if (!(service->svc_flags & SVC_forked)) {
		THREAD_ENTER();
		ERR_post (isc_svcnoexe, isc_arg_string,
			service->svc_service->serv_name, isc_arg_end); 
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
	Firebird::string	spb_sys_user_name;
	Firebird::string	spb_user_name;
	Firebird::string	spb_password;
	Firebird::string	spb_password_enc;
	Firebird::string	spb_command_line;
	Firebird::string	spb_network_protocol;
	Firebird::string    spb_remote_address;
#ifdef TRUSTED_SERVICES
	Firebird::string	spb_trusted_login;
#endif
	USHORT				spb_version;

	Serv_param_block() : spb_version(0) { }
};


static const TEXT* error_string(const TEXT* data, USHORT length);
static const TEXT* error_string(const Firebird::string& s);
static void conv_switches(Firebird::ClumpletReader&, Firebird::string&);
static const TEXT* find_switch(int, const in_sw_tab_t*);
static bool process_switches(Firebird::ClumpletReader&, Firebird::string&);
static void get_options(Firebird::ClumpletReader&, Serv_param_block*);
static bool get_action_svc_bitmask(const Firebird::ClumpletReader&,
                                   const in_sw_tab_t*, Firebird::string&);
static void get_action_svc_string(const Firebird::ClumpletReader&,
                                  Firebird::string&);
static void get_action_svc_data(const Firebird::ClumpletReader&,
                                Firebird::string&);
static bool get_action_svc_parameter(UCHAR tag, const in_sw_tab_t*,
		                             Firebird::string&);

#ifdef SERVICE_THREAD
static UCHAR service_dequeue_byte(Service*);
static void service_enqueue_byte(UCHAR, Service*);
static USHORT service_add_one(USHORT i);
static USHORT service_empty(Service* service);
static USHORT service_full(Service* service);
static void service_fork(ThreadEntryPoint*, Service*);
#else
#ifndef WIN_NT
static void timeout_handler(void* service);
#endif
static void cleanup(Service* service);
static void service_fork(TEXT*, Service*);
static void io_error(const TEXT*, SLONG, const TEXT*, ISC_STATUS);
#endif
static void service_get(Service*, SCHAR *, USHORT, USHORT, USHORT, USHORT *);
static void service_put(Service*, const SCHAR*, USHORT);

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

static Firebird::Mutex svc_mutex, thd_mutex;

/* Service Functions */
#if defined(SERVICE_THREAD) && !defined(BOOT_BUILD)
#include "../burp/burp_proto.h"
#include "../alice/alice_proto.h"
int main_lock_print();
THREAD_ENTRY_DECLARE main_gstat(THREAD_ENTRY_PARAM arg);

#define MAIN_GBAK		BURP_main
#define MAIN_GFIX		ALICE_main
#define MAIN_LOCK_PRINT	main_lock_print
#define MAIN_GSTAT		main_gstat
#else
#define MAIN_GBAK		NULL
#define MAIN_GFIX		NULL
#define MAIN_LOCK_PRINT	NULL
#define MAIN_GSTAT		NULL
#define MAIN_GSEC		NULL
#endif

#if defined(SERVICE_THREAD) && !defined(EMBEDDED) && !defined(BOOT_BUILD)
#include "../utilities/gsec/gsec_proto.h"
#define MAIN_GSEC		GSEC_main
#else
#define MAIN_GSEC		NULL
#endif


void SVC_STATUS_ARG(ISC_STATUS*& status, const MsgFormat::safe_cell& value)
{
	using MsgFormat::safe_cell;
    
	switch (value.type)
	{
	case safe_cell::at_int64:
	case safe_cell::at_uint64:
		*status++ = isc_arg_number;
		*status++ = static_cast<SLONG>(value.i_value); // May truncate number!
		break;
	case safe_cell::at_str:
		{
			*status++ = isc_arg_string;
			const char* s = value.st_value.s_string;
			*status++ = (ISC_STATUS) error_string(s, s ? strlen(s) : 0);
		}
		break;
	case safe_cell::at_counted_str:
		{
			*status++ = isc_arg_string;
			const char* s = value.st_value.s_string;
			*status++ = (ISC_STATUS) error_string(s, value.st_value.s_len);
		}
		break;

	default:
		break;
	}
}


void SVC_STATUS_ARG(ISC_STATUS*& status, const char* value)
{
	*status++ = isc_arg_string;
	*status++ = (ISC_STATUS) error_string(value, value ? strlen(value) : 0);
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
	{ isc_action_svc_get_fb_log, "Get Log File", NULL, NULL,	SVC_read_fb_log, 0 },
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
			   const SCHAR*	spb_data)
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
	

/* Find the service by looking for an exact match. */
	const Firebird::string misc_buf(service_name,
		service_length ? service_length : strlen(service_name));
	serv_entry* serv;
	for (serv = services; serv->serv_name; serv++) {
		if (misc_buf == serv->serv_name)
			break;
	}

	if (!serv->serv_name)
#ifdef NOT_USED_OR_REPLACED
		ERR_post(isc_service_att_err, isc_arg_gds, isc_service_not_supported,
				 isc_arg_end);
#else
		ERR_post(isc_service_att_err, isc_arg_gds, isc_svcnotdef,
				 isc_arg_string, error_string(misc_buf),
				 isc_arg_end);
#endif

	JRD_get_thread_data();

/* If anything goes wrong, we want to be able to free any memory
   that may have been allocated. */
	Service* service = 0;
	bool useSecureDb = false;

	try {
 	Firebird::ClumpletWriter spb(Firebird::ClumpletReader::SpbAttach, MAX_DPB_SIZE, 
		reinterpret_cast<const UCHAR*>(spb_data), spb_length, isc_spb_current_version);

/* Insert internal switch SERVICE_THD_PARAM in the service parameter block. */
	/* dimitr: it looks that we shouldn't execute the below code
	           if the first switch of the command line is "-svc_re",
			   but I couldn't find where such a switch is specified
			   by any of the client tools, so it seems that in fact
			   it's not used at all. Hence I ignore this situation. */
	if (spb.find(isc_spb_command_line)) {
		Firebird::string cl;
		spb.getString(cl);
		if (cl.substr(0, 5) == "-svc ")
			cl.erase(0, 5);
		else if (cl.substr(0, 9) == "-svc_thd ")
			cl.erase(0, 9);
		cl.insert(0, ' ');
		cl.insert(0, SERVICE_THD_PARAM);
		spb.deleteClumplet();
		spb.insertString(isc_spb_command_line, cl);
	}

/* Process the service parameter block. */
	Serv_param_block options;
	get_options(spb, &options);

/* Perhaps checkout the user in the security database. */
	USHORT user_flag;
	if (!strcmp(serv->serv_name, "anonymous")) {
		user_flag = SVC_user_none;
	}
	else {
#ifdef TRUSTED_SERVICES
		static AmCache useTrusted = AM_UNKNOWN;
		if (useTrusted == AM_UNKNOWN)
		{
			Firebird::PathName authMethod(Config::getAuthMethod());
			useTrusted = (authMethod == AmTrusted || authMethod == AmMixed) ? 
				AM_ENABLED : AM_DISABLED;
		}

		if (options.spb_trusted_login.hasData() && (useTrusted == AM_ENABLED))
		{
			options.spb_user_name = options.spb_trusted_login;
		}
		else
#endif
		{
			if (!options.spb_user_name.hasData())
			{
				// user name and password are required while
				// attaching to the services manager
				ERR_post(isc_service_att_err, isc_arg_gds, isc_svcnouser, isc_arg_end);
			}
			else 
			{
				Firebird::string name; // unused after retrieved
				int id, group, node_id;

				const Firebird::string remote = options.spb_network_protocol +
							(options.spb_network_protocol.isEmpty() || 
							 options.spb_remote_address.isEmpty() ? "" : "/") +
										  options.spb_remote_address;
			
				useSecureDb = true;
				SecurityDatabase::initialize();
				SecurityDatabase::verifyUser(name, options.spb_user_name.nullStr(),
						                     options.spb_password.nullStr(), 
											 options.spb_password_enc.nullStr(),
											 &id, &group, &node_id, remote);
			}
		}

		if (options.spb_user_name.length() > USERNAME_LENGTH)
		{
			Firebird::status_exception::raise(isc_long_login, 
											  isc_arg_number, options.spb_user_name.length(), 
											  isc_arg_number, USERNAME_LENGTH, isc_arg_end);
		}

/* Check that the validated user has the authority to access this service */
		if (fb_utils::stricmp(options.spb_user_name.c_str(), SYSDBA_USER_NAME))
		{
			user_flag = SVC_user_any;
		}
		else {
			user_flag = SVC_user_dba | SVC_user_any;
		}
	}

/* move service switches in */
	Firebird::string switches;
	if (serv->serv_std_switches)
		switches = serv->serv_std_switches;
	if (options.spb_command_line.hasData() && serv->serv_std_switches)
		switches += " ";
	switches += options.spb_command_line;
	
/* Services operate outside of the context of databases.  Therefore
   we cannot use the JRD allocator. */
	service = FB_NEW(*getDefaultMemoryPool()) Service(serv, *getDefaultMemoryPool());

	service->svc_flags =
		(serv->serv_executable ? SVC_forked : 0) | (switches.hasData() ? SVC_cmd_line : 0);
	service->svc_perm_sw = switches;
	service->svc_user_flag = user_flag;
	service->svc_uses_security_database = useSecureDb;
	
#ifdef SERVICE_THREAD
	service->svc_stdout_head = 1;
	service->svc_stdout_tail = SVC_STDOUT_BUFFER_SIZE;
	service->svc_stdout = NULL;
#endif
	service->svc_spb_version = options.spb_version;
	service->svc_username = options.spb_user_name;
#ifdef TRUSTED_SERVICES
	service->svc_trusted_login = options.spb_trusted_login;
#endif

/* The password will be issued to the service threads on NT since
 * there is no OS authentication.  If the password is not yet
 * encrypted, then encrypt it before saving it (since there is no
 * decrypt function).
 */
	if (options.spb_password_enc.hasData()) 
	{
		service->svc_enc_password = options.spb_password_enc;
	}
	else if (options.spb_password.hasData()) 
	{
		service->svc_enc_password.resize(MAX_PASSWORD_LENGTH + 2);
		ENC_crypt(service->svc_enc_password.begin(), service->svc_enc_password.length(), 
				  options.spb_password.c_str(), PASSWORD_SALT);
		service->svc_enc_password.recalculate_length();
		service->svc_enc_password.erase(0, 2);
	}

/* If an executable is defined for the service, try to fork a new process.
 * Only do this if we are working with a version 1 service */

#ifndef SERVICE_THREAD
	if (serv->serv_executable && options.spb_version == isc_spb_version1)
#else
	if (serv->serv_thd && options.spb_version == isc_spb_version1)
#endif
	{
#ifndef SERVICE_THREAD
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
					 isc_svc_in_use, isc_arg_string, serv->serv_name, isc_arg_end);
		}
#endif
	}

	}	// try
	catch (const Firebird::Exception&) {
		if (useSecureDb)
		{
			SecurityDatabase::shutdown();
		}
		delete service;
		throw;
	}

	return service;
}


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

	if (service->svc_uses_security_database)
	{
		SecurityDatabase::shutdown();
	}

#ifdef SERVER_SHUTDOWN
	if (service->svc_do_shutdown) {
		THREAD_EXIT();
#ifdef UNIX
		kill(getpid(), 15);
#else
		JRD_shutdown_all(true);
		if (shutdown_fct) {
			(shutdown_fct) (shutdown_param);
		}
		else {
			exit(0);
		}
#endif //UNIX

		/* The shutdown operation is complete, just wait to die */
		while (true) {
			THREAD_YIELD();
		}
	}
#endif /* SERVER_SHUTDOWN */

#ifdef SERVICE_THREAD

/* Mark service as detached. */
/* If service thread is finished, cleanup memory being used by service. */

	SVC_finish(service, SVC_detached);

#else

/* Go ahead and cleanup memory being used by service */
	cleanup(service);

#endif
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


#ifdef SERVICE_THREAD
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
#endif /*SERVICE_THREAD*/


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

	SCHAR* start_info;

	if (*items == isc_info_length) {
		start_info = info;
		items++;
	}
	else {
		start_info = 0;
	}

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
				ERR_post(isc_bad_spb_form, isc_arg_end);
				break;
			}
		}

		switch ((item = *items++))
		{
		case isc_info_end:
			break;

#ifdef SERVER_SHUTDOWN
		case isc_info_svc_svr_db_info:
			if (service->svc_user_flag & SVC_user_dba)
			{
				UCHAR dbbuf[1024];
				ULONG num_dbs = 0;
				ULONG num_att = 0;

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
			else
				need_admin_privs(&status, "isc_info_svc_svr_db_info");

			break;

		case isc_info_svc_svr_online:
			*info++ = item;
			if (service->svc_user_flag & SVC_user_dba) {
				service->svc_do_shutdown = false;
				WHY_set_shutdown(SHUTDOWN_NONE);
			}
			else
				need_admin_privs(&status, "isc_info_svc_svr_online");
			break;

		case isc_info_svc_svr_offline:
			*info++ = item;
			if (service->svc_user_flag & SVC_user_dba) {
				service->svc_do_shutdown = true;
				WHY_set_shutdown(SHUTDOWN_ATTACH);
			}
			else
				need_admin_privs(&status, "isc_info_svc_svr_offline");
			break;
#endif /* SERVER_SHUTDOWN */

			/* The following 3 service commands (or items) stuff the response
			   buffer 'info' with values of environment variable FIREBIRD,
			   FIREBIRD_LOCK or FIREBIRD_MSG. If the environment variable
			   is not set then default value is returned.
			 */
		case isc_info_svc_get_env:
		case isc_info_svc_get_env_lock:
		case isc_info_svc_get_env_msg:
			if (service->svc_user_flag & SVC_user_dba)
			{
				switch (item) 
				{
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
			}
			else
			{
				need_admin_privs(&status, "isc_info_svc_get_env");
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

		case isc_info_svc_get_config:
			// TODO: iterate through all integer-based config values
			//		 and return them to the client
			break;
/*
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
			/* The server implementation - e.g. Firebird/sun4 */
			isc_format_implementation(IMPLEMENTATION, sizeof(buffer), buffer,
									  0, 0, NULL);
			info = INF_put_item(item, strlen(buffer), buffer, info, end);
			if (!info) {
				THREAD_ENTER();
				return 0;
			}

			break;


		case isc_info_svc_user_dbpath:
			if (service->svc_user_flag & SVC_user_dba)
			{
				/* The path to the user security database (security2.fdb) */
				SecurityDatabase::getPath(buffer);

				if (!(info = INF_put_item(item, strlen(buffer), buffer,
										  info, end)))
				{
					THREAD_ENTER();
					return 0;
				}
			}
			else
				need_admin_privs(&status, "isc_info_svc_user_dbpath");
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

		default:
			*status++ = isc_wish_list;
			*status++ = isc_arg_end;
			break;
		}

		if (service->svc_user_flag == SVC_user_none)
			break;
	}

	if (info < end)
		*info++ = isc_info_end;

	if (start_info && (end - info >= 7))
	{
		SLONG number = info - start_info;
		memmove(start_info + 7, start_info, number);
		USHORT length2 = INF_convert(number, buffer);
		INF_put_item(isc_info_length, length2, buffer, start_info, end, true);
	}

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
			if (service->svc_user_flag & SVC_user_dba)
			{
				ULONG num_att = 0;
				ULONG num_dbs = 0;
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
			/*
			 * Can not return error for service v.1 => simply ignore request
			else
				need_admin_privs(&status, "isc_info_svc_svr_db_info");
			 */
			break;

		case isc_info_svc_svr_online:
			*info++ = item;
			if (service->svc_user_flag & SVC_user_dba) {
				service->svc_do_shutdown = false;
				WHY_set_shutdown(SHUTDOWN_NONE);
				*info++ = 0;	/* Success */
			}
			else
				*info++ = 2;	/* No user authority */
			break;

		case isc_info_svc_svr_offline:
			*info++ = item;
			if (service->svc_user_flag & SVC_user_dba) {
				service->svc_do_shutdown = true;
				WHY_set_shutdown(SHUTDOWN_ATTACH);
				*info++ = 0;	/* Success */
			}
			else
				*info++ = 2;	/* No user authority */
			break;
#endif /* SERVER_SHUTDOWN */

			/* The following 3 service commands (or items) stuff the response
			   buffer 'info' with values of environment variable FIREBIRD,
			   FIREBIRD_LOCK or FIREBIRD_MSG. If the environment variable
			   is not set then default value is returned.
			 */
		case isc_info_svc_get_env:
		case isc_info_svc_get_env_lock:
		case isc_info_svc_get_env_msg:
            if (service->svc_user_flag & SVC_user_dba)
			{
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
			}
			/*
			 * * Can not return error for service v.1 => simply ignore request
			else
			{
				need_admin_privs(&status, "isc_info_svc_get_env");
			}
			 */
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
			*
			 * Can not return error for service v.1 => simply ignore request
			else
				need_admin_privs(&status, "isc_info_svc_default_config:");
			 *
			break;

		case isc_info_svc_set_config:
			*info++ = item;
			if (service->svc_user_flag & SVC_user_dba) {
				THREAD_ENTER();
				// TODO: set the config values
				THREAD_EXIT();
			}
			*
			 * Can not return error for service v.1 => simply ignore request
			else
				need_admin_privs(&status, "isc_info_svc_set_config:");
			 *
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
			/* The server implementation - e.g. Firebird/sun4 */

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
            if (service->svc_user_flag & SVC_user_dba)
            {
				/* The path to the user security database (security2.fdb) */
				SecurityDatabase::getPath(buffer);

				if (!(info = INF_put_item(item, strlen(buffer), buffer, info, end)))
				{
					THREAD_ENTER();
					return;
				}
			}
			/*
			 * Can not return error for service v.1 => simply ignore request
			else
				need_admin_privs(&status, "isc_info_svc_user_dbpath");
			 */
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


void* SVC_start(Service* service, USHORT spb_length, const SCHAR* spb_data)
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

	Firebird::ClumpletReader spb(Firebird::ClumpletReader::SpbStart, 
		reinterpret_cast<const UCHAR*>(spb_data), spb_length);

/* The name of the service is the first element of the buffer */
	const UCHAR svc_id = spb.getClumpTag();
	serv_entry* serv;
	for (serv = services; serv->serv_action; serv++)
		if (serv->serv_action == svc_id)
			break;

	if (!serv->serv_name)
		ERR_post(isc_service_att_err, isc_arg_gds, isc_service_not_supported, isc_arg_end);

/* currently we do not use "anonymous" service for any purposes but
   isc_service_query() */
	if (service->svc_user_flag == SVC_user_none) {
		ERR_post(isc_bad_spb_form, isc_arg_end);
	}

	thd_mutex.enter();
	if (service->svc_flags & SVC_thd_running) {
		thd_mutex.leave();
		ERR_post(isc_svc_in_use, isc_arg_string,
				 error_string(serv->serv_name, strlen(serv->serv_name)),
				 isc_arg_end);
	}
	/* Another service may have been started with this service block.
	 * If so, we must reset the service flags.
	 */
	service->svc_switches.erase();
	if (!(service->svc_flags & SVC_detached))
	{
		service->svc_flags = 0;
	}
	service->svc_flags |= SVC_thd_running;
	thd_mutex.leave();

	thread_db* tdbb = JRD_get_thread_data();

	if (!service->svc_perm_sw.hasData())
	{
		/* If svc_perm_sw is not used -- call a command-line parsing utility */
		conv_switches(spb, service->svc_switches);
	}
	else
	{
		/* Command line options (isc_spb_options) is used.
		 * Currently the only case in which it might happen is -- gbak utility
		 * is called with a "-server" switch.
		 */
		service->svc_switches = service->svc_perm_sw;
	}

/* Only need to add username and password information to those calls which need
 * to make a database connection
 */
	if (svc_id == isc_action_svc_backup ||
		svc_id == isc_action_svc_restore ||
		svc_id == isc_action_svc_repair ||
		svc_id == isc_action_svc_add_user ||
		svc_id == isc_action_svc_delete_user ||
		svc_id == isc_action_svc_modify_user ||
		svc_id == isc_action_svc_display_user ||
		svc_id == isc_action_svc_db_stats ||
		svc_id == isc_action_svc_properties)
	{
		/* add the username and password to the end of svc_switches if needed */
		if (service->svc_switches.hasData()) 
		{
#ifdef TRUSTED_SERVICES
			if (service->svc_trusted_login.hasData()) 
			{
				service->svc_switches += ' ';
				service->svc_switches += TRUSTED_USER_SWITCH;
				service->svc_switches += ' ';
				service->svc_switches += service->svc_username;
			}
			else
#endif
			{
				if (service->svc_username.hasData())
				{
					service->svc_switches += ' ';
					service->svc_switches += USERNAME_SWITCH;
					service->svc_switches += ' ';
					service->svc_switches += service->svc_username;
				}

				if (service->svc_enc_password.hasData())
				{
					service->svc_switches += ' ';
					service->svc_switches += PASSWORD_SWITCH;
					service->svc_switches += ' ';
					service->svc_switches += service->svc_enc_password;
				}
			}
		}
	}

// All services except for get_ib_log require switches
	spb.rewind();
	if ((!service->svc_switches.hasData()) && svc_id != isc_action_svc_get_fb_log) 
	{
		ERR_post(isc_bad_spb_form, isc_arg_end);
	}

// Do not let everyone look at server log
	if (svc_id == isc_action_svc_get_fb_log && !(service->svc_user_flag & SVC_user_dba))
    {
       	ERR_post(isc_adm_task_denied, isc_arg_end);
    }

#ifndef SERVICE_THREAD
	TEXT service_path[MAXPATHLEN];

	if (serv->serv_executable) {
		gds__prefix(service_path, serv->serv_executable);
		service->svc_flags = SVC_forked;
		service_fork(service_path, service);
	}

#else

/* Break up the command line into individual arguments. */
	service->parseSwitches();
	service->svc_argv[0] = (TEXT *)(serv->serv_thd);

/*
 * the service block can be reused hence free a memory from the
 * previous usage as well as init a status vector.
 */

	memset((void *) service->svc_status, 0, sizeof(ISC_STATUS_ARRAY));

	if (service->svc_stdout)
		gds__free(service->svc_stdout);

	service->svc_stdout = (UCHAR*)gds__alloc((SLONG) SVC_STDOUT_BUFFER_SIZE + 1);
/* FREE: at SVC_detach() */
	if (!service->svc_stdout)	/* NOMEM: */
	{
		ERR_post(isc_virmemexh, isc_arg_end);
	}

	if (serv->serv_thd) {
		// reset stdout buffer - fix for CORE-3220
		service->svc_stdout_head = 1;
		service->svc_stdout_tail = SVC_STDOUT_BUFFER_SIZE;

		service->svc_flags &= ~SVC_evnt_fired;
		THREAD_EXIT();

		gds__thread_start(serv->serv_thd, service, THREAD_medium, 0,
						  (void *) &service->svc_handle);

		// Check for the service being detached. This will prevent the thread
		// from waiting infinitely if the client goes away.
		while (!(service->svc_flags & SVC_detached)) 
		{
			// The semaphore will be released once the particular service 
			// has reached a point in which it can start to return
			// information to the client.  This will allow isc_service_start
			// to include in its status vector information about the service's
			// ability to start. 
			// This is needed since gds__thread_start will almost always succeed.
			if (service->svcStart.tryEnter(60))
			{
				// svc_started was called
				break;
			}
		}

		THREAD_ENTER();
	}
	else
	{
		ERR_post(isc_svcnotdef,
				isc_arg_string,
				error_string(serv->serv_name, strlen(serv->serv_name)),
				isc_arg_end);
	}

#endif /* SERVICE_THREAD */

	return reinterpret_cast<void*>(reserved);
}


THREAD_ENTRY_DECLARE SVC_read_fb_log(THREAD_ENTRY_PARAM arg)
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
#ifdef SERVICE_THREAD
	ISC_STATUS *status = service->svc_status;
	*status++ = isc_arg_gds;
#endif

	TEXT name[MAXPATHLEN];
	gds__prefix(name, LOGFILE);
	FILE* file = fopen(name, "r");
	if (file != NULL) {
#ifdef SERVICE_THREAD
		*status++ = FB_SUCCESS;
		*status++ = isc_arg_end;
#endif
		service->svc_started();
		svc_started = true;
		TEXT buffer[100];
		while (!feof(file) && !ferror(file)) {
			fgets(buffer, sizeof(buffer), file);
#ifdef SERVICE_THREAD
			SVC_fprintf(service, "%s", buffer);
#else
			service_put(service, buffer, sizeof(buffer));
#endif
		}
	}

	if (!file || file && ferror(file)) {
#ifdef SERVICE_THREAD
		*status++ = isc_sys_request;
		if (!file) {
			SVC_STATUS_ARG(status, "fopen");
		}
		else {
			SVC_STATUS_ARG(status, "fgets");
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

#ifdef SERVICE_THREAD
	SVC_finish(service, SVC_finished);
#else
	cleanup(service);
#endif
	return (FINI_OK);
}


static void get_options(Firebird::ClumpletReader&	spb,
						Serv_param_block*			options)
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
	const UCHAR p = spb.getBufferTag();
	if (p != isc_spb_version1 && p != isc_spb_current_version) {
		ERR_post(isc_bad_spb_form, isc_arg_gds, isc_wrospbver, isc_arg_end);
	}
	options->spb_version = p;

	for (spb.rewind(); !(spb.isEof()); spb.moveNext())
	{
		switch (spb.getClumpTag()) 
		{
		case isc_spb_sys_user_name:
			spb.getString(options->spb_sys_user_name);
			break;

		case isc_spb_user_name:
			spb.getString(options->spb_user_name);
			break;

		case isc_spb_password:
			spb.getString(options->spb_password);
			break;

		case isc_spb_password_enc:
			spb.getString(options->spb_password_enc);
			break;

#ifdef TRUSTED_SERVICES
		case isc_spb_trusted_auth:
			spb.getString(options->spb_trusted_login);
			break;
#endif

		case isc_spb_command_line:
			spb.getString(options->spb_command_line);
			break;

		case isc_spb_address_path: 
			{
				Firebird::ClumpletReader address_stack(Firebird::ClumpletReader::UnTagged, 
					spb.getBytes(), spb.getClumpLength());
				while (!address_stack.isEof()) {
					if (address_stack.getClumpTag() != isc_dpb_address) {
						address_stack.moveNext();
						continue;
					}
					Firebird::ClumpletReader address(Firebird::ClumpletReader::UnTagged, 
						address_stack.getBytes(), address_stack.getClumpLength());
					while (!address.isEof()) {
						switch (address.getClumpTag()) {
							case isc_dpb_addr_protocol:
								address.getString(options->spb_network_protocol);
								break;
							case isc_dpb_addr_endpoint:
								address.getString(options->spb_remote_address);
								break;
							default:
								break;
						}
						address.moveNext();
					}
					break;
				}
			}
			break;

		}
	}
}


#ifndef SERVICE_THREAD
static void io_error(const TEXT* string,
					 SLONG status,
					 const TEXT* filename, ISC_STATUS operation)
{
/**************************************
 *
 *	i o _ e r r o r
 *
 **************************************
 *
 * Functional description
 *	Report an I/O error.
 *
 **************************************/

	ERR_post(isc_io_error, isc_arg_string, string, isc_arg_string, filename,
			 isc_arg_gds, operation, SYS_ERR, status, isc_arg_end);
}
#endif


#ifdef SERVICE_THREAD
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
		service->svc_stdout[service_add_one(service->svc_stdout_tail)] = ch;
		service->svc_stdout_tail = service_add_one(service->svc_stdout_tail);
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

	// Break up the command line into individual arguments.
	service->parseSwitches();

	USHORT argc = service->svc_argc;
	service->svc_argv[0] = (TEXT *)(service_executable);

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
			service->svc_flags |= SVC_timeout;
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
#endif /* SERVICE_THREAD */


#ifndef SERVICE_THREAD

#ifdef WIN_NT

static void service_fork(TEXT* service_path, Service* service)
{
/**************************************
 *
 *	s e r v i c e _ f o r k		( W I N _ N T )
 *
 **************************************
 *
 * Functional description
 *	Startup a service. Just a stub.
 *
 **************************************/
	ERR_post(isc_service_att_err, isc_arg_gds, isc_service_not_supported, isc_arg_end);
}

static void service_get(
						Service* service,
						SCHAR * buffer,
						USHORT length,
						USHORT flags, USHORT timeout, USHORT * return_length)
{
/**************************************
 *
 *	s e r v i c e _ g e t		( W I N _ N T )
 *
 **************************************
 *
 * Functional description
 *	Get input from a service.
 *	Just a stub
 *
 **************************************/
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
 *	Send output to a service.
 *	Just a stub
 *
 **************************************/
}

#else

#ifdef DARWIN
#define NO_DOUBLE_VFORK
#endif

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
		io_error("pipe", errno, "", isc_io_create_err);

/* Probe service executable to see it if plausibly exists. */

	struct stat stat_buf;
	if (statistics(service_path, &stat_buf) == -1)
		io_error("stat", errno, service_path, isc_io_access_err);

/* Break up the command line into individual arguments. */
	service->parseSwitches();
	const char **argv = &service->svc_argv[0];
	*argv = service_path;

/* At last we can fork the sub-process.  If the fork succeeds, repeat
   it so that we don't have defunct processes hanging around. */

	THREAD_EXIT();

	int pid;
	
	switch (pid = vfork()) {
	case -1:
		THREAD_ENTER();
		ERR_post(isc_sys_request, isc_arg_string, "vfork", SYS_ERR, errno, isc_arg_end);
		break;

	case 0:
#ifndef NO_DOUBLE_VFORK
		if (vfork() > 0)
			_exit(FINI_OK);
#endif
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
			const char** s = argv;

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
		execvp(argv[0], const_cast<char* const*>(argv));
		_exit(FINI_ERROR);
	}

	close(pair1[1]);
	close(pair2[0]);

#ifndef NO_DOUBLE_VFORK
	waitpid(pid, NULL, 0);
#endif

	THREAD_ENTER();

	if (!(service->svc_input = fdopen(pair1[0], "r")) ||
		!(service->svc_output = fdopen(pair2[1], "w")))
	{
		io_error("fdopen", errno, "service path", isc_io_access_err);
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
			*buf++ = (flags & GET_LINE) && (c == '\n') ? ' ' : c;
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
			io_error("getc", errno_save, "service pipe", isc_io_read_err);
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
			io_error("putc", errno, "service pipe", isc_io_write_err);
	}

	if (fflush((FILE *) service->svc_output) == EOF)
		io_error("fflush", errno, "service pipe", isc_io_write_err);
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
#endif // !WIN_NT
#endif // SERVICE_THREAD


static void cleanup(Service* service)
{
/**************************************
 *
 *	c l e a n u p
 *
 **************************************
 *
 * Functional description
 *	Cleanup memory used by service.
 *
 **************************************/
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

	svc_mutex.enter();
	if (service && ((flag == SVC_finished) || (flag == SVC_detached)))
	{
		service->svc_flags |= flag;
		if ((service->svc_flags & SVC_finished) &&
			(service->svc_flags & SVC_detached))
		{
			cleanup(service);
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
	svc_mutex.leave();
}


static const TEXT* error_string(const TEXT* data, USHORT length)
{
/********************************************
 *
 *   e r r o r _ s t r i n g
 *
 ********************************************
 *
 * Functional Description:
 *     Uses ERR_string to save string data for the
 *     status vector
 * CVC: None of the callees check the pointer, so we do here as paranoid measure.
 * The chain is error_string->ERR_string->status_nstring->InterlockedStringsBuffer::alloc
 *   ->CircularBuffer::alloc->memcpy
 ********************************************/
	if (data)
		return ERR_string(data, length);

	data = "(null string)";
	return ERR_string(data, strlen(data));
}


static const TEXT* error_string(const Firebird::string& s)
{
	return ERR_string(s.c_str(), s.length());
}


static void conv_switches(Firebird::ClumpletReader& spb, Firebird::string& switches)
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
	spb.rewind();
	if (spb.getClumpTag() < isc_action_min || spb.getClumpTag() > isc_action_max)
		return;					/* error action not defined */

	// convert to string
	Firebird::string sw;
	if (! process_switches(spb, sw)) 
	{
		return;
	}

	sw.insert(0, ' ');
	sw.insert(0, SERVICE_THD_PARAM);

	switches = sw;
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


static bool process_switches(Firebird::ClumpletReader&	spb,
							 Firebird::string&			switches)
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
 **************************************/
	if (spb.getBufferLength() == 0)
		return false;

	spb.rewind();
	const UCHAR svc_action = spb.getClumpTag();
	spb.moveNext();

	Firebird::string burp_database, burp_backup;
	int burp_options = 0;
	bool found = false;

	do 
	{
		switch (svc_action) 
		{
		case isc_action_svc_delete_user:
		case isc_action_svc_display_user:
			if (!found) {
				if (!get_action_svc_parameter(svc_action, gsec_action_in_sw_table, switches))
				{
					return false;
				}
				else {
					if (spb.isEof() && svc_action == isc_action_svc_display_user)
					{
						// in case of "display all users" the spb buffer contains
						// nothing but isc_action_svc_display_user or isc_spb_dbname
						break;
					}

					if (spb.getClumpTag() != isc_spb_sec_username &&
						spb.getClumpTag() != isc_spb_dbname)
					{
						// unexpected item in service parameter block, expected @1
						ERR_post(isc_unexp_spb_form, isc_arg_string,
								 error_string(SPB_SEC_USERNAME,
												strlen(SPB_SEC_USERNAME)),
								 isc_arg_end);
					}
					found = true;
				}
			}

			switch (spb.getClumpTag()) 
			{
			case isc_spb_sql_role_name:
			case isc_spb_dbname:
				if (!get_action_svc_parameter(spb.getClumpTag(), gsec_in_sw_table, switches))
				{
					return false;
				}
				// fall through ....
			case isc_spb_sec_username:
				get_action_svc_string(spb, switches);
				break;
				
			default:
				return false;
				break;
			}
			break;

		case isc_action_svc_add_user:
		case isc_action_svc_modify_user:
			if (!found) {
				if (!get_action_svc_parameter(svc_action, gsec_action_in_sw_table, switches))
				{
					return false;
				}
				else {
					if (spb.getClumpTag() != isc_spb_sec_username) {
						// unexpected item in service parameter block, expected @1
						ERR_post(isc_unexp_spb_form, isc_arg_string,
								 error_string(SPB_SEC_USERNAME,
												strlen(SPB_SEC_USERNAME)),
								 isc_arg_end);
					}
					found = true;
				}
			}

			switch (spb.getClumpTag()) 
			{
			case isc_spb_sec_userid:
			case isc_spb_sec_groupid:
				if (!get_action_svc_parameter(spb.getClumpTag(), gsec_in_sw_table, switches))
				{
					return false;
				}
				get_action_svc_data(spb, switches);
				break;

			case isc_spb_sql_role_name:
			case isc_spb_sec_password:
			case isc_spb_sec_groupname:
			case isc_spb_sec_firstname:
			case isc_spb_sec_middlename:
			case isc_spb_sec_lastname:
			case isc_spb_dbname:
				if (!get_action_svc_parameter(spb.getClumpTag(), gsec_in_sw_table, switches))
				{
					return false;
				}
				// fall through ....
			case isc_spb_sec_username:
				get_action_svc_string(spb, switches);
				break;

			default:
				return false;
				break;
			}
			break;

		case isc_action_svc_db_stats:
			switch (spb.getClumpTag()) {
			case isc_spb_dbname:
				get_action_svc_string(spb, switches);
				break;

			case isc_spb_options:
				if (!get_action_svc_bitmask(spb, dba_in_sw_table, switches))
				{
					return false;
				}
				break;

			case isc_spb_command_line: 
				{
					Firebird::string s;
					spb.getString(s);
					switches += s;
					switches += ' ';
				}
				break;

			default:
				return false;
				break;
			}
			break;

		case isc_action_svc_backup:
		case isc_action_svc_restore:
			switch (spb.getClumpTag()) {
			case isc_spb_bkp_file:
                get_action_svc_string(spb, burp_backup);
				break;
			case isc_spb_dbname:
                get_action_svc_string(spb, burp_database);
				break;
			case isc_spb_options:
				burp_options |= spb.getInt();
				if (!get_action_svc_bitmask(spb, burp_in_sw_table, switches))
				{
					return false;
				}
				break;
			case isc_spb_bkp_length:
				get_action_svc_data(spb, burp_backup);
				break;
			case isc_spb_res_length:
				get_action_svc_data(spb, burp_database);
				break;
			case isc_spb_bkp_factor:
			case isc_spb_res_buffers:
			case isc_spb_res_page_size:
				if (!get_action_svc_parameter(spb.getClumpTag(), burp_in_sw_table, switches))
				{
					return false;
				}
				get_action_svc_data(spb, switches);
				break;
			case isc_spb_res_access_mode:
				if (!get_action_svc_parameter(*(spb.getBytes()), burp_in_sw_table, switches))
				{
					return false;
				}
				break;
			case isc_spb_verbose:
				if (!get_action_svc_parameter(spb.getClumpTag(), burp_in_sw_table, switches))
				{
					return false;
				}
				break;
			default:
				return false;
				break;
			}
			break;

		case isc_action_svc_repair:
		case isc_action_svc_properties:
			switch (spb.getClumpTag()) {
			case isc_spb_dbname:
                get_action_svc_string(spb, switches);
				break;
			case isc_spb_options:
				if (!get_action_svc_bitmask(spb, alice_in_sw_table, switches))
				{
					return false;
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
				if (!get_action_svc_parameter(spb.getClumpTag(), alice_in_sw_table, switches))
				{
					return false;
				}
				get_action_svc_data(spb, switches);
				break;
			case isc_spb_prp_write_mode:
			case isc_spb_prp_access_mode:
			case isc_spb_prp_reserve_space:
				if (!get_action_svc_parameter(*(spb.getBytes()), alice_in_sw_table, switches))
				{
					return false;
				}
				break;
			default:
				return false;
				break;
			}
			break;
		default:
			return false;
			break;
		}
		
		spb.moveNext();
	} while (! spb.isEof());

	// postfixes for burp
	switch (svc_action)
	{
	case isc_action_svc_backup:
		switches += (burp_database + burp_backup);
		break;
	case isc_action_svc_restore:
		if (! (burp_options & (isc_spb_res_create | isc_spb_res_replace)))
		{
			// user not specified create or replace database
			// default to create for restore
			switches += "-CREATE_DATABASE ";
		}
		switches += (burp_backup + burp_database);
		break;
	}

	switches.rtrim();
	return switches.length() > 0;
}


static bool get_action_svc_bitmask(const Firebird::ClumpletReader& spb,
								   const in_sw_tab_t* table,
                                   Firebird::string& switches)
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
	const int opt = spb.getInt();
	ISC_ULONG mask = 1;
	for (int count = (sizeof(ISC_ULONG) * 8) - 1; count--; mask <<= 1) 
	{
		if (opt & mask) 
		{
			const TEXT* s_ptr = find_switch((opt & mask), table);
			if (!s_ptr)
			{
				return false;
			}
			
			switches += '-';
			switches += s_ptr;
			switches += ' ';
		}
	}

	return true;
}


static void get_action_svc_string(const Firebird::ClumpletReader& spb,
								  Firebird::string& switches)
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
	Firebird::string s;
	spb.getString(s);
	switches.appendQuoted(s, SVC_TRMNTR);
	switches += ' ';
}


static void get_action_svc_data(const Firebird::ClumpletReader& spb,
                                Firebird::string& switches)
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
	Firebird::string s;
	s.printf("%"ULONGFORMAT" ", spb.getInt());
	switches += s;
}


static bool get_action_svc_parameter(UCHAR action, 
									 const in_sw_tab_t* table,
									 Firebird::string& switches)
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
	const TEXT* s_ptr = find_switch(action, table);
	if (!s_ptr)
	{
		return false;
	}

	switches += '-';
	switches += s_ptr;
	switches += ' ';

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

