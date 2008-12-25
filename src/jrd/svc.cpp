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
 * 2008		Alex Peshkoff - refactored services code for MT safe engine
 */

#include "firebird.h"
#include <stdio.h>
#include <string.h>
#include "../jrd/common.h"
#include "../jrd/file_params.h"
#include <stdarg.h>
#include "../jrd/jrd.h"
#include "../jrd/svc.h"
#include "../jrd/constants.h"
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
#include "../jrd/thread_proto.h"
#include "../jrd/why_proto.h"
#include "../jrd/utl_proto.h"
#include "../jrd/jrd_proto.h"
#include "../jrd/enc_proto.h"
#include "../utilities/gsec/gsecswi.h"
#include "../utilities/gstat/dbaswi.h"
#include "../utilities/nbackup/nbkswi.h"
#include "../common/classes/alloc.h"
#include "../common/classes/init.h"
#include "../common/classes/ClumpletWriter.h"
#include "../jrd/ibase.h"
#include "../common/utils_proto.h"
#include "../jrd/scl.h"
#include "../jrd/msg_encode.h"

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

#if !defined(WIN_NT)
#  include <signal.h>
#  include <sys/param.h>
#  include <sys/stat.h>
#  include <errno.h>
#else
#  include <windows.h>
#  include <io.h> // _open, _get_osfhandle
#  include <stdlib.h>
#  include <fcntl.h>
#  include <sys/stat.h>
#endif

#define statistics	stat

using namespace Firebird;

const int SVC_user_dba			= 2;
const int SVC_user_any			= 1;
const int SVC_user_none			= 0;

const int GET_LINE		= 1;
const int GET_EOF		= 2;
const int GET_BINARY	= 4;

const char* const SPB_SEC_USERNAME = "isc_spb_sec_username";

namespace {

	// Option block for service parameter block
	struct Options 
	{
		string spb_sys_user_name;
		string spb_user_name;
		string spb_password;
		string spb_password_enc;
		string spb_command_line;
		string spb_network_protocol;
		string spb_remote_address;
		string spb_trusted_login;
		string spb_address_path;
		USHORT spb_version;
		bool spb_trusted_role;
		bool spb_remote;

		// Parse service parameter block picking up options and things.
		explicit Options(ClumpletReader& spb) :
			spb_version(0),
			spb_trusted_role(false),
			spb_remote(false)
		{
			const UCHAR p = spb.getBufferTag();
			if (p != isc_spb_version1 && p != isc_spb_current_version) {
				ERR_post(Arg::Gds(isc_bad_spb_form) <<
						 Arg::Gds(isc_wrospbver));
			}
			spb_version = p;

			for (spb.rewind(); !(spb.isEof()); spb.moveNext())
			{
				switch (spb.getClumpTag())
				{
				case isc_spb_sys_user_name:
					spb.getString(spb_sys_user_name);
					break;

				case isc_spb_user_name:
					spb.getString(spb_user_name);
					break;

				case isc_spb_password:
					spb.getString(spb_password);
					break;

				case isc_spb_password_enc:
					spb.getString(spb_password_enc);
					break;

				case isc_spb_trusted_auth:
					spb.getString(spb_trusted_login);
					break;

				case isc_spb_trusted_role:
					spb_trusted_role = true;
					break;

				case isc_spb_command_line:
					spb.getString(spb_command_line);
					break;

				case isc_spb_address_path:
					spb.getString(spb_address_path);
					spb_remote = true;
					{
						ClumpletReader address_stack(ClumpletReader::UnTagged,
							spb.getBytes(), spb.getClumpLength());
						while (!address_stack.isEof())
						{
							if (address_stack.getClumpTag() != isc_dpb_address)
							{
								address_stack.moveNext();
								continue;
							}

							ClumpletReader address(ClumpletReader::UnTagged,
								address_stack.getBytes(), address_stack.getClumpLength());

							while (!address.isEof())
							{
								switch (address.getClumpTag())
								{
								case isc_dpb_addr_protocol:
									address.getString(spb_network_protocol);
									break;
								case isc_dpb_addr_endpoint:
									address.getString(spb_remote_address);
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
	};

	// Generic mutex to synchronize services
	GlobalPtr<Mutex> svc_mutex;

	// All that we need to shutdown service threads when shutdown in progress
	typedef Array<Jrd::Service*> AllServices;
	GlobalPtr<AllServices> allServices;	// protected by svc_mutex
	volatile bool svcShutdown = false;

	void put_status_arg(ISC_STATUS*& status, const MsgFormat::safe_cell& value)
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
			*status++ = isc_arg_string;
			*status++ = (ISC_STATUS) (IPTR) value.st_value.s_string;
			break;
		default:
			break;
		}
	}
} // anonymous namespace


using namespace Jrd;

void Service::parseSwitches()
{
	svc_parsed_sw = svc_switches;
	svc_parsed_sw.trim();
	argv.clear();
	argv.push("service");	// why not use it for argv[0]

	if (svc_parsed_sw.isEmpty())
	{
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
				svc_parsed_sw[i] = 0;
			}
			break;
		}
	}

	argv.push(svc_parsed_sw.c_str());

	for (const char* p = svc_parsed_sw.begin(); p < svc_parsed_sw.end(); ++p)
	{
		if (!*p)
		{
			argv.push(p + 1);
		}
	}
}

void Service::printf(const SCHAR* format, ...)
{
	// Ensure that service is not detached.
	if (svc_flags & SVC_detached)
	{
		return;
	}

	string buf;
	va_list arglist;
	va_start(arglist, format);
	buf.vprintf(format, arglist);
	va_end(arglist);

	const char* const end = buf.end();

	for (const char* p = buf.begin(); p < end && !(svc_flags & SVC_detached); ++p)
	{
		enqueueByte(*p);
	}
}

bool Service::isService()
{
	return true;
}

void Service::started()
{
	if (!(svc_flags & SVC_evnt_fired))
	{
		MutexLockGuard guard(svc_mutex);
		svc_flags |= SVC_evnt_fired;
		svcStart.release();
	}
}

void Service::finish()
{
	finish(SVC_finished);
}

void Service::putLine(char tag, const char* val)
{
	const size_t len = strlen(val) & 0xFFFF;
	enqueueByte(tag);
	enqueueByte(len);
	enqueueByte(len >> 8);

	for (size_t i = 0; i < len; i++)
	{
		enqueueByte(val[i]);
	}
}

void Service::putSLong(char tag, SLONG val)
{
	enqueueByte(tag);
	enqueueByte(val);
	enqueueByte(val >> 8);
	enqueueByte(val >> 16);
	enqueueByte(val >> 24);
}

void Service::putChar(char tag, char val)
{
	enqueueByte(tag);
	enqueueByte(val);
}

void Service::setServiceStatus(const ISC_STATUS* status_vector)
{
	if (checkForShutdown())
	{
		return;
	}

	if (status_vector != svc_status)
	{
		Arg::StatusVector svc(svc_status);
		Arg::StatusVector passed(status_vector);
		svc.append(passed);
		svc.copyTo(svc_status);
		makePermanentVector(svc_status);
	}
}

void Service::setServiceStatus(const USHORT facility, const USHORT errcode, const MsgFormat::SafeArg& args)
{
	if (checkForShutdown())
	{
		return;
	}

	// Append error codes to the status vector

	ISC_STATUS_ARRAY tmp_status;

	// stuff the status into temp buffer
	MOVE_CLEAR(tmp_status, sizeof(tmp_status));
	ISC_STATUS* status = tmp_status;
	*status++ = isc_arg_gds;
	*status++ = ENCODE_ISC_MSG(errcode, facility);
	int tmp_status_len = 3;

	// We preserve the five params of the old code.
	// Don't want to overflow the status vector.
	for (unsigned int loop = 0; loop < 5 && loop < args.getCount(); ++loop)
	{
		put_status_arg(status, args.getCell(loop));
		tmp_status_len += 2;
	}

	*status++ = isc_arg_end;

	if (svc_status[0] != isc_arg_gds ||
		(svc_status[0] == isc_arg_gds && svc_status[1] == 0 && svc_status[2] != isc_arg_warning))
	{
		memcpy(svc_status, tmp_status, sizeof(ISC_STATUS) * tmp_status_len);
	}
	else
	{
		int status_len = 0, warning_indx = 0;
		PARSE_STATUS(svc_status, status_len, warning_indx);
		if (status_len)
			--status_len;

		// check for duplicated error code
		bool duplicate = false;
		int i;
		for (i = 0; i < ISC_STATUS_LENGTH; i++)
		{
			if (svc_status[i] == isc_arg_end && i == status_len)
				break;			// end of argument list

			if (i && i == warning_indx)
				break;			// vector has no more errors

			if (svc_status[i] == tmp_status[1] && i != 0 &&
				svc_status[i - 1] != isc_arg_warning &&
				i + tmp_status_len - 2 < ISC_STATUS_LENGTH &&
				(memcmp(&svc_status[i], &tmp_status[1],
					sizeof(ISC_STATUS) * (tmp_status_len - 2)) == 0))
			{
				// duplicate found
				duplicate = true;
				break;
			}
		}

		if (!duplicate)
		{
			// if the status_vector has only warnings then adjust err_status_len
			int err_status_len = i;
			if (err_status_len == 2 && warning_indx != 0)
				err_status_len = 0;

			ISC_STATUS_ARRAY warning_status;
			int warning_count = 0;
			if (warning_indx)
			{
				// copy current warning(s) to a temp buffer
				MOVE_CLEAR(warning_status, sizeof(warning_status));
				memcpy(warning_status, &svc_status[warning_indx],
							sizeof(ISC_STATUS) * (ISC_STATUS_LENGTH - warning_indx));
				PARSE_STATUS(warning_status, warning_count, warning_indx);
			}

			// add the status into a real buffer right in between last error and first warning
			i = err_status_len + tmp_status_len;
			if (i < ISC_STATUS_LENGTH)
			{
				memcpy(&svc_status[err_status_len], tmp_status, sizeof(ISC_STATUS) * tmp_status_len);
				// copy current warning(s) to the status_vector
				if (warning_count && i + warning_count - 1 < ISC_STATUS_LENGTH)
				{
					memcpy(&svc_status[i - 1], warning_status, sizeof(ISC_STATUS) * warning_count);
				}
			}
		}
	}

	makePermanentVector(svc_status);
}

void Service::hidePasswd(ArgvType&, int)
{
	// no action
}

ISC_STATUS* Service::getStatus()
{
	return svc_status;
}

void Service::checkService()
{
	// no action
}

void Service::getAddressPath(ClumpletWriter& dpb)
{
	if (svc_address_path.hasData())
	{
		dpb.insertString(isc_dpb_address_path, svc_address_path);
	}
}

void Service::makePermanentVector(ISC_STATUS *s)
{
	MutexLockGuard guard(svc_mutex);

	if (!svc_strings_buffer)
	{
		svc_strings_buffer = FB_NEW(*getDefaultMemoryPool()) CircularStringsBuffer<MAXPATHLEN * 4>;
	}
	svc_strings_buffer->makePermanentVector(s, s);
}

void Service::need_admin_privs(Arg::StatusVector& status, const char* message)
{
	status << Arg::Gds(isc_insufficient_svc_privileges) << Arg::Str(message);
}

bool Service::ck_space_for_numeric(char*& info, const char* const end)
{
	if ((info + 1 + sizeof(ULONG)) > end)
    {
		if (info < end)
			*info++ = isc_info_truncated;
		return false;
	}
	return true;
}


#ifdef DEBUG
THREAD_ENTRY_DECLARE test_thread(THREAD_ENTRY_PARAM);
void test_cmd(USHORT, SCHAR *, TEXT **);
#define TEST_THREAD test_thread
#define TEST_CMD test_cmd
#else
#define TEST_THREAD NULL
#define TEST_CMD NULL
#endif

/* Service Functions */
#if !defined(BOOT_BUILD)
#include "../burp/burp_proto.h"
#include "../alice/alice_proto.h"
THREAD_ENTRY_DECLARE main_gstat(THREAD_ENTRY_PARAM arg);
#include "../utilities/nbackup/nbk_proto.h"

#define MAIN_GBAK		BURP_main
#define MAIN_GFIX		ALICE_main
#define MAIN_GSTAT		main_gstat
#define MAIN_NBAK		NBACKUP_main
#else
#define MAIN_GBAK		NULL
#define MAIN_GFIX		NULL
#define MAIN_GSTAT		NULL
#define MAIN_NBAK		NULL
#endif

#if !defined(EMBEDDED) && !defined(BOOT_BUILD)
#include "../utilities/gsec/gsec_proto.h"
#define MAIN_GSEC		GSEC_main
#else
#define MAIN_GSEC		NULL
#endif


struct serv_entry
{
	USHORT				serv_action;		// isc_action_svc_....
	const TEXT*			serv_name;			// old service name
	const TEXT*			serv_std_switches;	// old cmd-line switches
	ThreadEntryPoint*	serv_thd;			// thread to execute
};

static const serv_entry services[] =
{

	{ isc_action_max, "print_cache", "", NULL },
	{ isc_action_max, "print_locks", "", NULL },
	{ isc_action_max, "start_cache", "", NULL },
	{ isc_action_max, "analyze_database", "", MAIN_GFIX },
	{ isc_action_max, "backup", "-b", MAIN_GBAK },
	{ isc_action_max, "create", "-c", MAIN_GBAK },
	{ isc_action_max, "restore", "-r", MAIN_GBAK },
	{ isc_action_max, "gdef", "-svc", NULL },
	{ isc_action_max, "gsec", "-svc", MAIN_GSEC },
	{ isc_action_max, "disable_journal", "-disable", NULL },
	{ isc_action_max, "dump_journal", "-online_dump", NULL },
	{ isc_action_max, "enable_journal", "-enable", NULL },
	{ isc_action_max, "monitor_journal", "-console", NULL },
	{ isc_action_max, "query_server", NULL, NULL },
	{ isc_action_max, "start_journal", "-server", NULL },
	{ isc_action_max, "stop_cache", "-shut -cache", MAIN_GFIX },
	{ isc_action_max, "stop_journal", "-console", NULL },
	{ isc_action_max, "anonymous", NULL, NULL },

	// NEW VERSION 2 calls, the name field MUST be different from those names above
	{ isc_action_max, "service_mgr", NULL, NULL },
	{ isc_action_svc_backup, "Backup Database", NULL, MAIN_GBAK },
	{ isc_action_svc_restore, "Restore Database", NULL, MAIN_GBAK },
	{ isc_action_svc_repair, "Repair Database", NULL, MAIN_GFIX },
	{ isc_action_svc_add_user, "Add User", NULL, MAIN_GSEC },
	{ isc_action_svc_delete_user, "Delete User", NULL, MAIN_GSEC },
	{ isc_action_svc_modify_user, "Modify User", NULL, MAIN_GSEC },
	{ isc_action_svc_display_user, "Display User", NULL, MAIN_GSEC },
	{ isc_action_svc_properties, "Database Properties", NULL, MAIN_GFIX },
	{ isc_action_svc_lock_stats, "Lock Stats", NULL, TEST_THREAD },
	{ isc_action_svc_db_stats, "Database Stats", NULL, MAIN_GSTAT },
	{ isc_action_svc_get_fb_log, "Get Log File", NULL, Service::readFbLog },
	{ isc_action_svc_nbak, "Incremental Backup Database", NULL, MAIN_NBAK },
	{ isc_action_svc_nrest, "Incremental Restore Database", NULL, MAIN_NBAK },
/* actions with no names are undocumented */
	{ isc_action_svc_set_config, NULL, NULL, TEST_THREAD },
	{ isc_action_svc_default_config, NULL, NULL, TEST_THREAD },
	{ isc_action_svc_set_env, NULL, NULL, TEST_THREAD },
	{ isc_action_svc_set_env_lock, NULL, NULL, TEST_THREAD },
	{ isc_action_svc_set_env_msg, NULL, NULL, TEST_THREAD },
	{ 0, NULL, NULL, NULL }
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


Service::Service(const TEXT* service_name, USHORT spb_length, const UCHAR* spb_data)
	: svc_parsed_sw(getPool()),
	svc_handle(0), svc_stdout_head(1), svc_stdout_tail(SVC_STDOUT_BUFFER_SIZE),
	svc_resp_alloc(getPool()), svc_resp_buf(0), svc_resp_ptr(0), svc_resp_buf_len(0),
	svc_resp_len(0), svc_flags(0), svc_user_flag(0), svc_spb_version(0), svc_do_shutdown(false),
	svc_username(getPool()), svc_enc_password(getPool()),
	svc_trusted_login(getPool()), svc_trusted_role(false), svc_uses_security_database(false),
	svc_switches(getPool()), svc_perm_sw(getPool()), svc_address_path(getPool()),
	svc_strings_buffer(NULL)
{
	memset(svc_status, 0, sizeof svc_status);

	{	// scope
		// Account service block in global array
		MutexLockGuard guard(svc_mutex);
		checkForShutdown();
		allServices->add(this);
	}

	// Since this moment we should remove this service from allServices in case of error thrown
	try
	{
		// If the service name begins with a slash, ignore it.
		if (*service_name == '/' || *service_name == '\\') {
			service_name++;
		}

		// Find the service by looking for an exact match.
		const string svcname(service_name);
		const serv_entry* serv;
		for (serv = services; serv->serv_name; serv++) {
			if (svcname == serv->serv_name)
				break;
		}

		if (!serv->serv_name) {
			status_exception::raise(Arg::Gds(isc_service_att_err) <<
									Arg::Gds(isc_svcnotdef) << Arg::Str(svcname));
		}

		// Process the service parameter block.
		ClumpletReader spb(ClumpletReader::SpbAttach, spb_data, spb_length);
		Options options(spb);

		// Perhaps checkout the user in the security database.
		SecurityDatabase::InitHolder siHolder;
		USHORT user_flag;
		if (!strcmp(serv->serv_name, "anonymous")) {
			user_flag = SVC_user_none;
		}
		else {
			// If we have embedded service connection, let's check for unix OS auth
			if (!options.spb_trusted_login.hasData() && !options.spb_remote &&
				!options.spb_user_name.hasData())
			{
				if (ISC_get_user(&options.spb_trusted_login, NULL, NULL, NULL)) {
					options.spb_trusted_login = SYSDBA_USER_NAME;
				}
			}

			if (options.spb_trusted_login.hasData()) {
				options.spb_user_name = options.spb_trusted_login;
			}
			else {
				// If we have embedded service connection, let's check for environment
				if (!options.spb_remote) {
					if (!options.spb_user_name.hasData()) {
						fb_utils::readenv(ISC_USER, options.spb_user_name);
					}
					if (!options.spb_password.hasData()) {
						fb_utils::readenv(ISC_PASSWORD, options.spb_password);
					}
				}

				if (!options.spb_user_name.hasData()) {
					// user name and password are required while
					// attaching to the services manager
					status_exception::raise(Arg::Gds(isc_service_att_err) << Arg::Gds(isc_svcnouser));
				}

				string name; // unused after retrieved
				int id, group, node_id;

				const string remote = options.spb_network_protocol +
					(options.spb_network_protocol.isEmpty() ||
						options.spb_remote_address.isEmpty() ? "" : "/") + 
					options.spb_remote_address;

				SecurityDatabase::verifyUser(name, options.spb_user_name.nullStr(),
						                     options.spb_password.nullStr(),
											 options.spb_password_enc.nullStr(),
											 &id, &group, &node_id, remote);
				svc_uses_security_database = true;
			}

			if (options.spb_user_name.length() > USERNAME_LENGTH) {
				status_exception::raise(Arg::Gds(isc_long_login) <<
					Arg::Num(options.spb_user_name.length()) << Arg::Num(USERNAME_LENGTH));
			}

			// Check that the validated user has the authority to access this service
			string uName(options.spb_user_name);
			uName.upper();
			if ((uName != SYSDBA_USER_NAME) && !options.spb_trusted_role) {
				user_flag = SVC_user_any;
			}
			else {
				user_flag = SVC_user_dba | SVC_user_any;
			}
		}

		// move service switches in
		string switches;
		if (serv->serv_std_switches)
			switches = serv->serv_std_switches;
		if (options.spb_command_line.hasData() && serv->serv_std_switches)
			switches += ' ';
		switches += options.spb_command_line;

		svc_flags = switches.hasData() ? SVC_cmd_line : 0;
		svc_perm_sw = switches;
		svc_user_flag = user_flag;
		svc_service = serv;
		svc_spb_version = options.spb_version;
		svc_username = options.spb_user_name;
		svc_trusted_login = options.spb_trusted_login;
		svc_trusted_role = options.spb_trusted_role;
		svc_address_path = options.spb_address_path;

		// The password will be issued to the service threads on NT since
		// there is no OS authentication.  If the password is not yet
		// encrypted, then encrypt it before saving it (since there is no
		// decrypt function).
		if (options.spb_password_enc.hasData())
		{
			svc_enc_password = options.spb_password_enc;
		}
		else if (options.spb_password.hasData())
		{
			svc_enc_password.resize(MAX_PASSWORD_LENGTH + 2);
			ENC_crypt(svc_enc_password.begin(), svc_enc_password.length(),
					  options.spb_password.c_str(), PASSWORD_SALT);
			svc_enc_password.recalculate_length();
			svc_enc_password.erase(0, 2);
		}

		// If an executable is defined for the service, try to fork a new thread.
		// Only do this if we are working with a version 1 service
		if (serv->serv_thd && options.spb_version == isc_spb_version1)
		{
			start(serv->serv_thd);
		}

		if (svc_uses_security_database)
		{
			siHolder.clear();
		}
	}
	catch (const Exception&)
	{
		removeFromAllServices();
		throw;
	}
}


static THREAD_ENTRY_DECLARE svcShutdownThread(THREAD_ENTRY_PARAM)
{
	if (fb_shutdown(10 * 1000 /* 10 seconds */, fb_shutrsn_services) == FB_SUCCESS)
	{
		InstanceControl::registerShutdown(0);
		exit(0);
	}

	return 0;
}


void Service::detach()
{
	// save it cause after call to finish() we can't access class members any more
	bool localDoShutdown = svc_do_shutdown;

	if (svc_uses_security_database)
	{
		SecurityDatabase::shutdown();
	}

	// Mark service as detached.
	finish(SVC_detached);

	if (localDoShutdown)
	{
		// run in separate thread to avoid blocking in remote
		gds__thread_start(svcShutdownThread, 0, 0, 0, 0);
	}
}


Service::~Service()
{
	if (svc_handle)
	{
		THD_detach(svc_handle);
		svc_handle = 0;
	}

	removeFromAllServices();
}


void Service::removeFromAllServices()
{
	MutexLockGuard guard(svc_mutex);
	AllServices& all(allServices);

	for (unsigned int pos = 0; pos < all.getCount(); ++pos)
	{
		if (all[pos] == this)
		{
			all.remove(pos);
			break;
		}
	}
}


bool Service::checkForShutdown()
{
	if (svcShutdown)
	{
		MutexLockGuard guard(svc_mutex);

		if (svc_flags & SVC_shutdown)
		{
			// Here we avoid multiple exceptions thrown
			return true;
		}

		svc_flags |= SVC_shutdown;
		status_exception::raise(Arg::Gds(isc_att_shutdown));
	}

	return false;
}


void Service::shutdownServices()
{
	svcShutdown = true;

	MutexLockGuard guard(svc_mutex);
	AllServices& all(allServices);

	for (unsigned int pos = 0; pos < all.getCount(); )
	{
		if (all[pos]->svc_flags & SVC_thd_running)
		{
			svc_mutex->leave();
			THD_sleep(1);
			svc_mutex->enter();
			pos = 0;
			continue;
		}

		++pos;
	}
}


ISC_STATUS Service::query2(thread_db* tdbb,
						   USHORT send_item_length,
						   const SCHAR* send_items,
						   USHORT recv_item_length,
						   const SCHAR* recv_items,
						   USHORT buffer_length,
						   SCHAR* info)
{
	SCHAR item;
	char buffer[MAXPATHLEN];
	USHORT l, length, version, get_flags;

	// Setup the status vector
	Arg::StatusVector status;

	// Process the send portion of the query first.
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
				l = (USHORT) gds__vax_integer(reinterpret_cast<const UCHAR*>(items), 2);
				items += 2;
				if (items + l <= end_items) {
					switch (item) {
					case isc_info_svc_line:
						put(items, l);
						break;
					case isc_info_svc_message:
						put(items - 3, l + 3);
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

	// Process the receive portion of the query now.
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
		 */
		if (svc_user_flag == SVC_user_none)
		{
			switch (*items)
			{
			case isc_info_svc_get_config:
			case isc_info_svc_dump_pool_info:
				break;
			default:
				status_exception::raise(Arg::Gds(isc_bad_spb_form));
				break;
			}
		}

		switch ((item = *items++))
		{
		case isc_info_end:
			break;

		case isc_info_svc_svr_db_info:
			if (svc_user_flag & SVC_user_dba)
			{
				UCHAR dbbuf[1024];
				ULONG num_dbs = 0;
				ULONG num_att = 0;

				*info++ = item;
				UCHAR* const ptr =
					JRD_num_attachments(dbbuf, sizeof(dbbuf), JRD_info_dbnames, &num_att, &num_dbs);
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
				const TEXT* ptr2 = reinterpret_cast<const TEXT*>(ptr);
				if (ptr2) {
					USHORT num = (USHORT) isc_vax_integer(ptr2, sizeof(USHORT));
					fb_assert(num == num_dbs);
					ptr2 += sizeof(USHORT);
					for (; num; num--) {
						length = (USHORT) isc_vax_integer(ptr2, sizeof(USHORT));
						ptr2 += sizeof(USHORT);
						if (!(info = INF_put_item(isc_spb_dbname, length, ptr2, info, end)))
						{
							if (ptr != dbbuf)
								gds__free(ptr);	// memory has been allocated by JRD_num_attachments()
							return 0;
						}
						ptr2 += length;
					}

					if (ptr != dbbuf)
						gds__free(ptr);	// memory has been allocated by JRD_num_attachments()
				}

				if (info < end)
					*info++ = isc_info_flag_end;
			}
			else
				need_admin_privs(status, "isc_info_svc_svr_db_info");

			break;

		case isc_info_svc_svr_online:
			*info++ = item;
			if (svc_user_flag & SVC_user_dba) {
				svc_do_shutdown = false;
				WHY_set_shutdown(false);
			}
			else
				need_admin_privs(status, "isc_info_svc_svr_online");
			break;

		case isc_info_svc_svr_offline:
			*info++ = item;
			if (svc_user_flag & SVC_user_dba) {
				svc_do_shutdown = true;
				WHY_set_shutdown(true);
			}
			else
				need_admin_privs(status, "isc_info_svc_svr_offline");
			break;

			/* The following 3 service commands (or items) stuff the response
			   buffer 'info' with values of environment variable FIREBIRD,
			   FIREBIRD_LOCK or FIREBIRD_MSG. If the environment variable
			   is not set then default value is returned.
			 */
		case isc_info_svc_get_env:
		case isc_info_svc_get_env_lock:
		case isc_info_svc_get_env_msg:
			if (svc_user_flag & SVC_user_dba)
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

				// Note: it is safe to use strlen to get a length of "buffer"
				// because gds_prefix[_lock|_msg] return a zero-terminated
				// string.
				info = INF_put_item(item, strlen(buffer), buffer, info, end);
				if (!info)
				{
					return 0;
				}
			}
			else
			{
				need_admin_privs(status, "isc_info_svc_get_env");
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
			if (svc_user_flag & SVC_user_dba) {
				// TODO: reset the config values to defaults
			}
			else
				need_admin_privs(status, "isc_info_svc_default_config");
			break;

		case isc_info_svc_set_config:
			*info++ = item;
			if (svc_user_flag & SVC_user_dba) {
				// TODO: set the config values
			}
			else {
				need_admin_privs(status, "isc_info_svc_set_config");
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
			if (svc_flags & SVC_thd_running)
				ADD_SPB_NUMERIC(info, TRUE)
			else
				ADD_SPB_NUMERIC(info, FALSE)

			break;

		case isc_info_svc_server_version:
			/* The version of the server engine */
			info = INF_put_item(item, strlen(GDS_VERSION), GDS_VERSION, info, end);
			if (!info) {
				return 0;
			}
			break;

		case isc_info_svc_implementation:
			/* The server implementation - e.g. Firebird/sun4 */
			isc_format_implementation(IMPLEMENTATION, sizeof(buffer), buffer,
									  0, 0, NULL);
			info = INF_put_item(item, strlen(buffer), buffer, info, end);
			if (!info) {
				return 0;
			}

			break;


		case isc_info_svc_user_dbpath:
			if (svc_user_flag & SVC_user_dba)
			{
				/* The path to the user security database (security2.fdb) */
				SecurityDatabase::getPath(buffer);

				if (!(info = INF_put_item(item, strlen(buffer), buffer, info, end)))
				{
					return 0;
				}
			}
			else
				need_admin_privs(status, "isc_info_svc_user_dbpath");
			break;

		case isc_info_svc_response:
			svc_resp_len = 0;
			if (info + 4 >= end) {
				*info++ = isc_info_truncated;
				break;
			}
			put(&item, 1);
			get(&item, 1, GET_BINARY, 0, &length);
			get(buffer, 2, GET_BINARY, 0, &length);
			l = (USHORT) gds__vax_integer(reinterpret_cast<UCHAR*>(buffer), 2);
			length = MIN(end - (info + 5), l);
			get(info + 3, length, GET_BINARY, 0, &length);
			info = INF_put_item(item, length, info + 3, info, end);
			if (length != l) {
				*info++ = isc_info_truncated;
				l -= length;
				if (l > svc_resp_buf_len) {
					try {
						svc_resp_buf = svc_resp_alloc.getBuffer(l);
					}
					catch (const BadAlloc&) {	// NOMEM:
						DEV_REPORT("SVC_query: out of memory");
						// NOMEM: not really handled well
						l = 0;	// set the length to zero
					}
					svc_resp_buf_len = l;
				}
				get(reinterpret_cast<char*>(svc_resp_buf), l, GET_BINARY, 0, &length);
				svc_resp_ptr = svc_resp_buf;
				svc_resp_len = l;
			}
			break;

		case isc_info_svc_response_more:
			if ( (l = length = svc_resp_len) )
				length = MIN(end - (info + 5), l);
			if (!
				(info =
				 INF_put_item(item, length, reinterpret_cast<const char*>(svc_resp_ptr), info, end)))
			{
				return 0;
			}
			svc_resp_ptr += length;
			svc_resp_len -= length;
			if (length != l)
				*info++ = isc_info_truncated;
			break;

		case isc_info_svc_total_length:
			put(&item, 1);
			get(&item, 1, GET_BINARY, 0, &length);
			get(buffer, 2, GET_BINARY, 0, &length);
			l = (USHORT) gds__vax_integer(reinterpret_cast<UCHAR*>(buffer), 2);
			get(buffer, l, GET_BINARY, 0, &length);
			if (!(info = INF_put_item(item, length, buffer, info, end))) {
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

			switch (item)
			{
			case isc_info_svc_line:
				get_flags = GET_LINE;
				break;
			case isc_info_svc_to_eof:
				get_flags = GET_EOF;
				break;
			default:
				get_flags = GET_BINARY;
				break;
			}

			get(info + 3, end - (info + 5), get_flags, timeout, &length);

			/* If the read timed out, return the data, if any, & a timeout
			   item.  If the input buffer was not large enough
			   to store a read to eof, return the data that was read along
			   with an indication that more is available. */

			if (!(info = INF_put_item(item, length, info + 3, info, end))) {
				return 0;
			}

			if (svc_flags & SVC_timeout)
			{
				*info++ = isc_info_svc_timeout;
			}
			else
			{
				if (!length && !(svc_flags & SVC_finished))
				{
					*info++ = isc_info_data_not_ready;
				}
				else if (item == isc_info_svc_to_eof && !(svc_flags & SVC_finished))
				{
					*info++ = isc_info_truncated;
				}
			}
			break;

		default:
			status << Arg::Gds(isc_wish_list);
			break;
		}

		if (svc_user_flag == SVC_user_none)
			break;
	}

	if (info < end)
		*info = isc_info_end;

	if (start_info && (end - info >= 7))
	{
		const SLONG number = 1 + (info - start_info);
		memmove(start_info + 7, start_info, number);
		USHORT length2 = INF_convert(number, buffer);
		INF_put_item(isc_info_length, length2, buffer, start_info, end);
	}

	if (!(svc_flags & SVC_thd_running))
	{
		finish(SVC_finished);
	}

	status.copyTo(tdbb->tdbb_status_vector);
	return tdbb->tdbb_status_vector[1];
}


void Service::query(USHORT			send_item_length,
					const SCHAR*	send_items,
					USHORT			recv_item_length,
					const SCHAR*	recv_items,
					USHORT			buffer_length,
					SCHAR*			info)
{
	SCHAR item, *p;
	char buffer[256];
	TEXT PathBuffer[MAXPATHLEN];
	USHORT l, length, version, get_flags;

	// Process the send portion of the query first.
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
						put(items, l);
						break;
					case isc_info_svc_message:
						put(items - 3, l + 3);
						break;
					case isc_info_svc_timeout:
						timeout = (USHORT) gds__vax_integer(reinterpret_cast<const UCHAR*>(items), l);
						break;
					case isc_info_svc_version:
						version = (USHORT) gds__vax_integer(reinterpret_cast<const UCHAR*>(items), l);
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

	// Process the receive portion of the query now.
	const SCHAR* const end = info + buffer_length;

	items = recv_items;
	const SCHAR* const end_items2 = items + recv_item_length;
	while (items < end_items2 && *items != isc_info_end)
	{
		switch ((item = *items++))
		{
		case isc_info_end:
			break;

		case isc_info_svc_svr_db_info:
			if (svc_user_flag & SVC_user_dba)
			{
				ULONG num_att = 0;
				ULONG num_dbs = 0;
				JRD_num_attachments(NULL, 0, JRD_info_none, &num_att, &num_dbs);
				length = INF_convert(num_att, buffer);
				info = INF_put_item(item, length, buffer, info, end);
				if (!info) {
					return;
				}
				length = INF_convert(num_dbs, buffer);
				info = INF_put_item(item, length, buffer, info, end);
				if (!info) {
					return;
				}
			}
			/*
			 * Can not return error for service v.1 => simply ignore request
			else
				need_admin_privs(status, "isc_info_svc_svr_db_info");
			 */
			break;

		case isc_info_svc_svr_online:
			*info++ = item;
			if (svc_user_flag & SVC_user_dba) {
				svc_do_shutdown = false;
				WHY_set_shutdown(false);
				*info++ = 0;	/* Success */
			}
			else
				*info++ = 2;	/* No user authority */
			break;

		case isc_info_svc_svr_offline:
			*info++ = item;
			if (svc_user_flag & SVC_user_dba) {
				svc_do_shutdown = true;
				WHY_set_shutdown(true);
				*info++ = 0;	/* Success */
			}
			else
				*info++ = 2;	/* No user authority */
			break;

			/* The following 3 service commands (or items) stuff the response
			   buffer 'info' with values of environment variable FIREBIRD,
			   FIREBIRD_LOCK or FIREBIRD_MSG. If the environment variable
			   is not set then default value is returned.
			 */
		case isc_info_svc_get_env:
		case isc_info_svc_get_env_lock:
		case isc_info_svc_get_env_msg:
			if (svc_user_flag & SVC_user_dba)
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

				// Note: it is safe to use strlen to get a length of "buffer"
				// because gds_prefix[_lock|_msg] return a zero-terminated
				// string.
				if (!(info = INF_put_item(item, strlen(PathBuffer), PathBuffer, info, end)))
					return;
			}
			/*
			 * Can not return error for service v.1 => simply ignore request
			else
				need_admin_privs(status, "isc_info_svc_get_env");
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
			if (svc_user_flag & SVC_user_dba) {
				// TODO: reset the config values to defaults
			}
			*
			 * Can not return error for service v.1 => simply ignore request
			else
				need_admin_privs(status, "isc_info_svc_default_config:");
			 *
			break;

		case isc_info_svc_set_config:
			*info++ = item;
			if (svc_user_flag & SVC_user_dba) {
				// TODO: set the config values
			}
			*
			 * Can not return error for service v.1 => simply ignore request
			else
				need_admin_privs(status, "isc_info_svc_set_config:");
			 *
			break;
*/
		case isc_info_svc_version:
			/* The version of the service manager */

			length = INF_convert(SERVICE_VERSION, buffer);
			info = INF_put_item(item, length, buffer, info, end);
			if (!info)
				return;
			break;

		case isc_info_svc_capabilities:
			/* bitmask defining any specific architectural differences */

			length = INF_convert(SERVER_CAPABILITIES_FLAG, buffer);
			info = INF_put_item(item, length, buffer, info, end);
			if (!info)
				return;
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
				return;
			}
			break;


		case isc_info_svc_user_dbpath:
            if (svc_user_flag & SVC_user_dba)
            {
				/* The path to the user security database (security2.fdb) */
				SecurityDatabase::getPath(buffer);

				if (!(info = INF_put_item(item, strlen(buffer), buffer, info, end)))
				{
					return;
				}
			}
			/*
			 * Can not return error for service v.1 => simply ignore request
			else
				need_admin_privs(status, "isc_info_svc_user_dbpath");
			 */
			break;

		case isc_info_svc_response:
			svc_resp_len = 0;
			if (info + 4 > end)
			{
				*info++ = isc_info_truncated;
				break;
			}
			put(&item, 1);
			get(&item, 1, GET_BINARY, 0, &length);
			get(buffer, 2, GET_BINARY, 0, &length);
			l = (USHORT) gds__vax_integer(reinterpret_cast<UCHAR*>(buffer), 2);
			length = MIN(end - (info + 4), l);
			get(info + 3, length, GET_BINARY, 0, &length);
			info = INF_put_item(item, length, info + 3, info, end);
			if (length != l)
			{
				*info++ = isc_info_truncated;
				l -= length;
				if (l > svc_resp_buf_len)
				{
					try {
						svc_resp_buf = svc_resp_alloc.getBuffer(l);
					}
					catch (const BadAlloc&) {	// NOMEM:
						DEV_REPORT("SVC_query: out of memory");
						// NOMEM: not really handled well
						l = 0;	// set the length to zero
					}
					svc_resp_buf_len = l;
				}
				get(reinterpret_cast<char*>(svc_resp_buf), l, GET_BINARY, 0, &length);
				svc_resp_ptr = svc_resp_buf;
				svc_resp_len = l;
			}
			break;

		case isc_info_svc_response_more:
			if ( (l = length = svc_resp_len) )
				length = MIN(end - (info + 4), l);
			if (!(info = 
				INF_put_item(item, length, reinterpret_cast<const char*>(svc_resp_ptr), info, end)))
			{
				return;
			}
			svc_resp_ptr += length;
			svc_resp_len -= length;
			if (length != l)
				*info++ = isc_info_truncated;
			break;

		case isc_info_svc_total_length:
			put(&item, 1);
			get(&item, 1, GET_BINARY, 0, &length);
			get(buffer, 2, GET_BINARY, 0, &length);
			l = (USHORT) gds__vax_integer(reinterpret_cast<UCHAR*>(buffer), 2);
			get(buffer, l, GET_BINARY, 0, &length);
			if (!(info = INF_put_item(item, length, buffer, info, end)))
			{
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
			get(info + 3, end - (info + 4), get_flags, timeout, &length);

			/* If the read timed out, return the data, if any, & a timeout
			   item.  If the input buffer was not large enough
			   to store a read to eof, return the data that was read along
			   with an indication that more is available. */

			info = INF_put_item(item, length, info + 3, info, end);

			if (svc_flags & SVC_timeout)
				*info++ = isc_info_svc_timeout;
			else
			{
				if (!length && !(svc_flags & SVC_finished))
					*info++ = isc_info_data_not_ready;
				else
				{
					if (item == isc_info_svc_to_eof && !(svc_flags & SVC_finished))
						*info++ = isc_info_truncated;
				}
			}
			break;
		}
	}

	if (info < end)
	{
		*info = isc_info_end;
	}

	if (!(svc_flags & SVC_thd_running))
	{
		finish(SVC_finished);
	}
}


void Service::start(USHORT spb_length, const UCHAR* spb_data)
{
	ClumpletReader spb(ClumpletReader::SpbStart, spb_data, spb_length);

/* The name of the service is the first element of the buffer */
	const UCHAR svc_id = spb.getClumpTag();
	const serv_entry* serv;
	for (serv = services; serv->serv_action; serv++)
	{
		if (serv->serv_action == svc_id)
			break;
	}

	if (!serv->serv_name)
		status_exception::raise(Arg::Gds(isc_service_att_err) << Arg::Gds(isc_service_not_supported));

/* currently we do not use "anonymous" service for any purposes but
   isc_service_query() */
	if (svc_user_flag == SVC_user_none) {
		status_exception::raise(Arg::Gds(isc_bad_spb_form));
	}

	{ // scope for locked svc_mutex
		MutexLockGuard guard(svc_mutex);

		if (svc_flags & SVC_thd_running) {
			status_exception::raise(Arg::Gds(isc_svc_in_use) << Arg::Str(serv->serv_name));
		}

		/* Another service may have been started with this service block.
		 * If so, we must reset the service flags.
		 */
		svc_switches.erase();
		if (!(svc_flags & SVC_detached))
		{
			svc_flags = 0;
		}
	}

	if (!svc_perm_sw.hasData())
	{
		/* If svc_perm_sw is not used -- call a command-line parsing utility */
		conv_switches(spb, svc_switches);
	}
	else
	{
		/* Command line options (isc_spb_options) is used.
		 * Currently the only case in which it might happen is -- gbak utility
		 * is called with a "-server" switch.
		 */
		svc_switches = svc_perm_sw;
	}

/* Only need to add username and password information to those calls which need
 * to make a database connection
 */
	if (svc_id == isc_action_svc_backup ||
		svc_id == isc_action_svc_restore ||
		svc_id == isc_action_svc_nbak ||
		svc_id == isc_action_svc_nrest ||
		svc_id == isc_action_svc_repair ||
		svc_id == isc_action_svc_add_user ||
		svc_id == isc_action_svc_delete_user ||
		svc_id == isc_action_svc_modify_user ||
		svc_id == isc_action_svc_display_user ||
		svc_id == isc_action_svc_db_stats ||
		svc_id == isc_action_svc_properties)
	{
		/* add the username and password to the end of svc_switches if needed */
		if (svc_switches.hasData())
		{
			if (svc_trusted_login.hasData())
			{
				string auth = "-";
				auth += TRUSTED_USER_SWITCH;
				auth += ' ';
				auth += svc_username;
				auth += ' ';
				if (svc_trusted_role)
				{
					auth += "-";
					svc_switches += TRUSTED_ROLE_SWITCH;
					auth += ' ';
				}
				svc_switches = auth + svc_switches;
			}
			else
			{
				// No need repeating user validation in service worker thread
				if (svc_username.hasData())
				{
					string auth = "-";
					auth += TRUSTED_USER_SWITCH;
					auth += ' ';
					auth += svc_username;
					auth += ' ';
					svc_switches = auth + svc_switches;
				}
			}
		}
	}

	// All services except for get_ib_log require switches
	spb.rewind();
	if ((!svc_switches.hasData()) && svc_id != isc_action_svc_get_fb_log)
	{
		status_exception::raise(Arg::Gds(isc_bad_spb_form));
	}

	// Do not let everyone look at server log
	if (svc_id == isc_action_svc_get_fb_log && !(svc_user_flag & SVC_user_dba))
    {
       	status_exception::raise(Arg::Gds(isc_adm_task_denied));
    }


	// Break up the command line into individual arguments.
	parseSwitches();

	// The service block can be reused hence init a status vector.
	memset((void *) svc_status, 0, sizeof(ISC_STATUS_ARRAY));

	if (serv->serv_thd) {
		{	// scope
			MutexLockGuard guard(svc_mutex);
			svc_flags &= ~SVC_evnt_fired;
			svc_flags |= SVC_thd_running;
		}

		gds__thread_start(serv->serv_thd, this, THREAD_medium, 0, &svc_handle);

		// Check for the service being detached. This will prevent the thread
		// from waiting infinitely if the client goes away.
		while (!(svc_flags & SVC_detached))
		{
			// The semaphore will be released once the particular service
			// has reached a point in which it can start to return
			// information to the client.  This will allow isc_service_start
			// to include in its status vector information about the service's
			// ability to start.
			// This is needed since gds__thread_start will almost always succeed.
			if (svcStart.tryEnter(60))
			{
				// started() was called
				break;
			}
		}
	}
	else
	{
		status_exception::raise(Arg::Gds(isc_svcnotdef) << Arg::Str(serv->serv_name));
	}
}


THREAD_ENTRY_DECLARE Service::readFbLog(THREAD_ENTRY_PARAM arg)
{
	Service* service = (Service*) arg;
	service->readFbLog();
	return 0;
}

void Service::readFbLog()
{
	bool svc_started = false;

	TEXT name[MAXPATHLEN];
	gds__prefix(name, LOGFILE);
	FILE* file = fopen(name, "r");
	if (file != NULL) {
		fb_utils::init_status(svc_status);
		started();
		svc_started = true;
		TEXT buffer[100];
		while (!feof(file) && !ferror(file)) {
			fgets(buffer, sizeof(buffer), file);
			output(buffer);
		}
	}

	if (!file || file && ferror(file)) {
		(Arg::Gds(isc_sys_request) << Arg::Str(file ? "fgets" : "fopen") <<
									  SYS_ERR(errno)).copyTo(svc_status);
		StringsBuffer::makeEnginePermanentVector(svc_status);
		if (!svc_started)
		{
			started();
		}
	}

	if (file)
		fclose(file);

	finish(SVC_finished);
}


USHORT Service::add_one(USHORT i)
{
	return ((i % SVC_STDOUT_BUFFER_SIZE) + 1);
}


bool Service::empty() const
{
	return add_one(svc_stdout_tail) == svc_stdout_head;
}


bool Service::full() const
{
	return add_one(add_one(svc_stdout_tail)) == svc_stdout_head;
}


UCHAR Service::dequeueByte()
{
	const UCHAR ch = svc_stdout[svc_stdout_head];
	svc_stdout_head = add_one(svc_stdout_head);

	return ch;
}


void Service::enqueueByte(const UCHAR ch)
{
	if (checkForShutdown())
	{
		return;
	}

	// Wait for space in buffer while service is not detached.
	while (full() && !(svc_flags & SVC_detached)) {
		THREAD_SLEEP(1);
		if (checkForShutdown())
		{
			return;
		}
	}

	// Ensure that service is not detached.
	if (!(svc_flags & SVC_detached)) {
		svc_stdout[add_one(svc_stdout_tail)] = ch;
		svc_stdout_tail = add_one(svc_stdout_tail);
	}
}


void Service::start(ThreadEntryPoint* service_thread)
{
	// Break up the command line into individual arguments.
	parseSwitches();

	if (svc_service && svc_service->serv_name)
	{
		argv[0] = svc_service->serv_name;
	}

	gds__thread_start(service_thread, this, THREAD_medium, 0, &svc_handle);
}


void Service::get(SCHAR* buffer, USHORT length, USHORT flags, USHORT timeout, USHORT* return_length)
{
#ifdef HAVE_GETTIMEOFDAY
	struct timeval start_time, end_time;
	GETTIMEOFDAY(&start_time);
#else
	time_t start_time, end_time;
	time(&start_time);
#endif

	*return_length = 0;

	{	// scope
		MutexLockGuard guard(svc_mutex);
		svc_flags &= ~SVC_timeout;
	}

	while (length) {
		if (empty())
			THREAD_SLEEP(1);
#ifdef HAVE_GETTIMEOFDAY
		GETTIMEOFDAY(&end_time);
		const time_t elapsed_time = end_time.tv_sec - start_time.tv_sec;
#else
		time(&end_time);
		const time_t elapsed_time = end_time - start_time;
#endif
		if ((timeout) && (elapsed_time >= timeout))
		{
			MutexLockGuard guard(svc_mutex);
			svc_flags &= SVC_timeout;
			return;
		}

		while (!empty() && length > 0)
		{
			const int ch = dequeueByte();
			length--;

			/* If returning a line of information, replace all new line
			 * characters with a space.  This will ensure that the output is
			 * consistent when returning a line or to eof
			 */
			if ((flags & GET_LINE) && (ch == '\n')) {
				buffer[(*return_length)++] = ' ';
				return;
			}

			buffer[(*return_length)++] = ch;
		}

		if (empty() && (svc_flags & SVC_finished))
			return;
	}
}


void Service::put(const SCHAR* buffer, USHORT length)
{
	// Nothing
}


void Service::finish(USHORT flag)
{
	if (flag == SVC_finished || flag == SVC_detached)
	{
		MutexLockGuard guard(svc_mutex);

		svc_flags |= flag;
		if (! (svc_flags & SVC_thd_running))
		{
			svc_flags |= SVC_finished;
		}
		if (svc_flags & SVC_finished && svc_flags & SVC_detached)
		{
			delete this;
			return;
		}
		if (svc_flags & SVC_finished)
		{
			svc_flags &= ~SVC_thd_running;
			if (svc_handle)
			{
				THD_detach(svc_handle);
				svc_handle = 0;
			}
		}
	}
}


void Service::conv_switches(ClumpletReader& spb, string& switches)
{
	spb.rewind();
	if (spb.getClumpTag() < isc_action_min || spb.getClumpTag() > isc_action_max) {
		return;					/* error - action not defined */
	}

	// convert to string
	string sw;
	if (! process_switches(spb, sw)) {
		return;
	}

	switches = sw;
}


const TEXT* Service::find_switch(int in_spb_sw, const in_sw_tab_t* table)
{
	for (const in_sw_tab_t* in_sw_tab = table; in_sw_tab->in_sw_name; in_sw_tab++)
	{
		if (in_spb_sw == in_sw_tab->in_spb_sw)
			return in_sw_tab->in_sw_name;
	}

	return NULL;
}


bool Service::process_switches(ClumpletReader& spb, string& switches)
{
	if (spb.getBufferLength() == 0)
		return false;

	spb.rewind();
	const UCHAR svc_action = spb.getClumpTag();
	spb.moveNext();

	string burp_database, burp_backup;
	int burp_options = 0;

	string nbk_database, nbk_file;
	int nbk_level = -1;

	bool found = false;

	do
	{
		switch (svc_action)
		{
		case isc_action_svc_nbak:
		case isc_action_svc_nrest:
			found = true;

			switch (spb.getClumpTag())
			{
			case isc_spb_dbname:
				if (nbk_database.hasData())
				{
					(Arg::Gds(isc_unexp_spb_form) << Arg::Str("only one isc_spb_dbname")).raise();
				}
				get_action_svc_string(spb, nbk_database);
				break;

			case isc_spb_nbk_level:
				if (nbk_level >= 0)
				{
					(Arg::Gds(isc_unexp_spb_form) << Arg::Str("only one isc_spb_nbk_level")).raise();
				}
				nbk_level = spb.getInt();
				break;

			case isc_spb_nbk_file:
				if (nbk_file.hasData() && svc_action != isc_action_svc_nrest)
				{
					(Arg::Gds(isc_unexp_spb_form) << Arg::Str("only one isc_spb_nbk_file")).raise();
				}
				get_action_svc_string(spb, nbk_file);
				break;

			case isc_spb_options:
				if (!get_action_svc_bitmask(spb, nbackup_in_sw_table, switches))
				{
					return false;
				}
				break;
			}
			break;

		case isc_action_svc_delete_user:
		case isc_action_svc_display_user:
			if (!found)
			{
				if (!get_action_svc_parameter(svc_action, gsec_action_in_sw_table, switches))
				{
					return false;
				}

				if (spb.isEof() && svc_action == isc_action_svc_display_user)
				{
					// in case of "display all users" the spb buffer contains
					// nothing but isc_action_svc_display_user or isc_spb_dbname
					break;
				}

				if (spb.getClumpTag() != isc_spb_sec_username && spb.getClumpTag() != isc_spb_dbname)
				{
					// unexpected item in service parameter block, expected @1
					status_exception::raise(Arg::Gds(isc_unexp_spb_form) << Arg::Str(SPB_SEC_USERNAME));
				}

				found = true;
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
			}
			break;

		case isc_action_svc_add_user:
		case isc_action_svc_modify_user:
			if (!found)
			{
				if (!get_action_svc_parameter(svc_action, gsec_action_in_sw_table, switches))
				{
					return false;
				}

				if (spb.getClumpTag() != isc_spb_sec_username) {
					// unexpected item in service parameter block, expected @1
					status_exception::raise(Arg::Gds(isc_unexp_spb_form) << Arg::Str(SPB_SEC_USERNAME));
				}
				found = true;
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
					string s;
					spb.getString(s);
					switches += s;
					switches += ' ';
				}
				break;

			default:
				return false;
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
				if (!get_action_svc_bitmask(spb, reference_burp_in_sw_table, switches))
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
				if (!get_action_svc_parameter(spb.getClumpTag(), reference_burp_in_sw_table, switches))
				{
					return false;
				}
				get_action_svc_data(spb, switches);
				break;
			case isc_spb_res_access_mode:
				if (!get_action_svc_parameter(*(spb.getBytes()), reference_burp_in_sw_table, switches))
				{
					return false;
				}
				break;
			case isc_spb_verbose:
				if (!get_action_svc_parameter(spb.getClumpTag(), reference_burp_in_sw_table, switches))
				{
					return false;
				}
				break;
			default:
				return false;
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
			case isc_spb_prp_force_shutdown:
			case isc_spb_prp_attachments_shutdown:
			case isc_spb_prp_transactions_shutdown:
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
			case isc_spb_prp_shutdown_mode:
			case isc_spb_prp_online_mode:
				if (get_action_svc_parameter(spb.getClumpTag(), alice_in_sw_table, switches))
				{
					unsigned int val = spb.getInt();
					if (val >= FB_NELEM(alice_mode_sw_table))
					{
						return false;
					}
					switches += alice_mode_sw_table[val];
					switches += " ";
					break;
				}
				return false;
			default:
				return false;
			}
			break;
		default:
			return false;
		}

		spb.moveNext();
	} while (! spb.isEof());

	// postfixes for burp & nbackup
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

	case isc_action_svc_nbak:
	case isc_action_svc_nrest:
		if (nbk_database.isEmpty())
		{
			(Arg::Gds(isc_missing_required_spb) << Arg::Str("isc_spb_dbname")).raise();
		}
		if (nbk_file.isEmpty())
		{
			(Arg::Gds(isc_missing_required_spb) << Arg::Str("isc_spb_nbk_file")).raise();
		}
		if (!get_action_svc_parameter(svc_action, nbackup_action_in_sw_table, switches))
		{
			return false;
		}
		if (svc_action == isc_action_svc_nbak)
		{
			if (nbk_level < 0)
			{
				(Arg::Gds(isc_missing_required_spb) << Arg::Str("isc_spb_nbk_level")).raise();
			}
			string temp;
			temp.printf("%d ", nbk_level);
			switches += temp;
		}
		switches += nbk_database;
		switches += nbk_file;
		break;
	}

	switches.rtrim();
	return switches.length() > 0;
}


bool Service::get_action_svc_bitmask(const ClumpletReader& spb,
									 const in_sw_tab_t* table,
									 string& switches)
{
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


void Service::get_action_svc_string(const ClumpletReader& spb,
									string& switches)
{
	string s;
	spb.getString(s);
	addStringWithSvcTrmntr(s, switches);
}


void Service::get_action_svc_data(const ClumpletReader& spb,
								  string& switches)
{
	string s;
	s.printf("%"ULONGFORMAT" ", spb.getInt());
	switches += s;
}


bool Service::get_action_svc_parameter(UCHAR action,
									   const in_sw_tab_t* table,
									   string& switches)
{
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
