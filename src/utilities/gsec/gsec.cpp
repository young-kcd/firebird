/*
 *
 *	PROGRAM:	Security data base manager
 *	MODULE:		gsec.cpp
 *	DESCRIPTION:	Main line routine
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>
#include "../jrd/common.h"
#include "../jrd/ibase.h"
#include "../jrd/gds_proto.h"
#include "../jrd/msg_encode.h"
#include "../jrd/isc_f_proto.h"
#include "../utilities/gsec/gsec.h"
#include "../utilities/gsec/gsec_proto.h"
#include "../jrd/jrd_pwd.h"
#include "../jrd/license.h"
#include "../jrd/svc.h"
#include "../jrd/svc_proto.h"
#include "../utilities/gsec/secur_proto.h"
#include "../utilities/gsec/gsecswi.h"
#include "../utilities/common/cmd_util_proto.h"
#include "../common/classes/ClumpletWriter.h"

#include "../utilities/gsec/call_service.h"
#include "../common/utils_proto.h"
#include "../common/classes/MsgPrint.h"

using MsgFormat::SafeArg;


#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef WIN_NT
#include <io.h>
#endif

#if defined (WIN95)
static bool fAnsiCP = false;
#define TRANSLATE_CP(a) if (!fAnsiCP) CharToOem(a, a)
#else
#define TRANSLATE_CP(a)
#endif

const int MAXARGS	= 20;		/* max number of args allowed on command line */
const int MAXSTUFF	= 1000;		/* longest interactive command line */

#ifndef SERVICE_THREAD
class tsec *gdsec;
#endif

static int common_main(int, fb_utils::arg_string*, Jrd::pfn_svc_output, Jrd::Service*);
static void util_output(const SCHAR*, ...);
static int util_print(const SCHAR*, ...);
static int vutil_print(const SCHAR*, va_list);

static void data_print(void*, const internal_user_data*, bool);
static bool get_line(int*, SCHAR**, TEXT*, size_t);
static bool get_switches(int, fb_utils::arg_string*, const in_sw_tab_t*, tsec*, bool*);
static SSHORT parse_cmd_line(int, fb_utils::arg_string*, tsec*);
static void printhelp(void);
#ifndef SERVICE_THREAD
static int output_main(Jrd::Service*, const UCHAR*);
#endif
inline void msg_get(USHORT, TEXT*);
static void get_security_error(ISC_STATUS*, int);
static void insert_error(ISC_STATUS*, ISC_STATUS);

void inline gsec_exit(int code, tsec* tdsec)
{
	tdsec->tsec_exit_code = code;
	if (tdsec->tsec_throw)
		Firebird::LongJump::raise();
}

#ifdef SERVICE_THREAD
THREAD_ENTRY_DECLARE GSEC_main(THREAD_ENTRY_PARAM arg)
{
/**********************************************
 *
 *    G S E C _ m a i n
 *
 **********************************************
 * Functional Description:
 *   Entrypoint for GSEC via the services manager
 **********************************************/
	Jrd::Service* service = (Jrd::Service*)arg;
	const int exit_code = common_main(service->svc_argc, service->svc_argv.begin(),
						  SVC_output, service);

/* Mark service thread as finished. */
/* If service is detached, cleanup memory being used by service. */
	SVC_finish(service, Jrd::SVC_finished);

	return (THREAD_ENTRY_RETURN)(IPTR)(exit_code);
}

#else

int CLIB_ROUTINE main( int argc, char* argv[])
{
/**************************************
 *
 *	m a i n
 *
 **************************************
 *
 * Functional description
 *	If there is no command line, prompt for one, read it
 *	and make an artificial argc/argv.   Otherwise, pass
 *	the specified argc/argv to SECURITY_exec_line (see below).
 *
 **************************************/
 	return common_main(argc, argv, output_main, NULL);
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
//	fprintf(stderr, "%s", output_buf);
	fprintf(stdout, "%s", output_buf);
	return 0;
}

inline void envPick(TEXT* dest, size_t size, const TEXT* var)
{
	if (dest && (!dest[0]))
	{
		Firebird::string val;
		if (fb_utils::readenv(var, val))
			val.copyTo(dest, size);
	}
}

#endif /* SERVICE_THREAD */

int common_main(int argc,
				fb_utils::arg_string argv[],
				Jrd::pfn_svc_output output_proc,
				Jrd::Service* output_data)
{
/**************************************
 *
 *	c o m m o n _ m a i n
 *
 **************************************
 *
 * Functional description
 *	If there is no command line, prompt for one, read it
 *	and make an artificial argc/argv.   Otherwise, pass
 *	the specified argc/argv to SECURITY_exec_line (see below).
 *
 **************************************/
#ifndef SERVICE_THREAD
	TEXT stuff[MAXSTUFF];		/* a place to put stuff */
#endif

#ifdef VMS
	argc = VMS_parse(&argv, argc);
#endif

	tsec tsecInstance(output_proc, output_data);
	tsec* tdsec = &tsecInstance;
	tsec::putSpecific(tdsec);

	internal_user_data u;
	tdsec->tsec_user_data = &u;

	try {
	// Perform some special handling when run as a Firebird service.  The
	// first switch can be "-svc" (lower case!) or it can be "-svc_re" followed
	// by 3 file descriptors to use in re-directing stdin, stdout and stderr.

	tdsec->tsec_throw = true;
	tdsec->tsec_interactive = true;
	internal_user_data* user_data = tdsec->tsec_user_data;

	if (argc > 1 && !strcmp(argv[1], "-svc")) {
		tdsec->tsec_service_gsec = true;
		argv++;
		argc--;
	}
#ifdef SERVICE_THREAD
	else if (argc > 1 && !strcmp(argv[1], "-svc_thd")) {
		tdsec->tsec_service_gsec = true;
		tdsec->tsec_service_thd = true;
		tdsec->tsec_service_blk = output_data;
		tdsec->tsec_status = tdsec->tsec_service_blk->svc_status;
		argv++;
		argc--;
	}
#endif
	else if (argc > 4 && !strcmp(argv[1], "-svc_re")) {
		tdsec->tsec_service_gsec = true;
		long redir_in = atol(argv[2]);
		long redir_out = atol(argv[3]);
		long redir_err = atol(argv[4]);
#ifdef WIN_NT
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

	ISC_STATUS* status = tdsec->tsec_status;
	SSHORT ret = parse_cmd_line(argc, argv, tdsec);
	Firebird::PathName databaseName;
	bool databaseNameEntered = user_data->database_name_entered;
	if (user_data->database_name_entered)
	{
		databaseName = user_data->database_name;
	}
	else
	{
		TEXT database_name[MAXPATHLEN];
		SecurityDatabase::getPath(database_name);
		databaseName = database_name;
	}
	
	Firebird::PathName serverName;
	bool useServices = false;
	switch (ISC_extract_host(databaseName, serverName, true))
	{
	case ISC_PROTOCOL_TCPIP:
		serverName += ":";
		useServices = true;
		break;
	case ISC_PROTOCOL_WLAN:
		serverName = "\\\\" + serverName + "\\";
		useServices = true;
		break;
	}
		
	if (! useServices)
	{
		serverName = "";
	}
	databaseName.copyTo(user_data->database_name, sizeof(user_data->database_name));
	
#ifdef SUPERCLIENT
	useServices = true;
#else //SUPERCLIENT
	FB_API_HANDLE db_handle = 0;

	if (! useServices) 
	{
		Firebird::ClumpletWriter dpb(Firebird::ClumpletReader::Tagged, MAX_DPB_SIZE, isc_dpb_version1);
		dpb.insertByte(isc_dpb_gsec_attach, 1); // not 0 - yes, I'm gsec

#ifdef TRUSTED_SERVICES
		if (user_data->dba_trust_user_name_entered) 
		{
			dpb.insertString(isc_dpb_trusted_auth, 
				user_data->dba_trust_user_name, strlen(user_data->dba_trust_user_name));
		}
		else
#endif
		{
#ifdef TRUSTED_AUTH
			if (user_data->trusted_auth) 
			{
				dpb.insertTag(isc_dpb_trusted_auth);
			}
#endif
			if (user_data->dba_user_name_entered) 
			{
				dpb.insertString(isc_dpb_user_name, 
					user_data->dba_user_name, strlen(user_data->dba_user_name));
			}

			if (user_data->dba_password_entered) 
			{
				dpb.insertString(tdsec->tsec_service_gsec ? 
								isc_dpb_password_enc : isc_dpb_password, 
					user_data->dba_password, strlen(user_data->dba_password));
			}
		}

		if (user_data->sql_role_name_entered) {
			dpb.insertString(isc_dpb_sql_role_name, 
				user_data->sql_role_name, strlen(user_data->sql_role_name));
		}

		if (isc_attach_database(status, 0, databaseName.c_str(), &db_handle, 
				dpb.getBufferLength(), 
				reinterpret_cast<const char*>(dpb.getBuffer())))
		{
			GSEC_error_redirect(status, GsecMsg15);
		}
	}
#endif //SUPERCLIENT

	isc_svc_handle sHandle = 0;
	if (useServices)
	{
#ifndef SERVICE_THREAD
#ifdef TRUSTED_AUTH
		if (!(user_data->trusted_auth))
#endif
		{
			envPick(user_data->dba_user_name, sizeof user_data->dba_user_name, "ISC_USER");
			envPick(user_data->dba_password, sizeof user_data->dba_password, "ISC_PASSWORD");
		}
#endif //SERVICE_THREAD
		sHandle = attachRemoteServiceManager(
					status,
					user_data->dba_user_name,
					user_data->dba_password,
#ifdef TRUSTED_AUTH
					user_data->trusted_auth,
#else
					false,
#endif
					serverName.c_str());
		if (! sHandle)
		{
			GSEC_error_redirect(status, GsecMsg15);
		}
	}

	if (!tdsec->tsec_interactive) {
		if (ret == 0) {
#ifdef SERVICE_THREAD
			/* Signal the start of the service here ONLY if we are displaying users
			 * since the number of users may exceed the service buffer.  This
			 * will cause the service to wait for the client to request data.  However,
			 * if the server is not signaled, then the client can never request anything */
			if (user_data->operation == DIS_OPER)
				tdsec->tsec_service_blk->svc_started();
#endif
#ifndef SUPERCLIENT
			if (! useServices)
			{
				ret = SECURITY_exec_line(status, db_handle, 
							user_data, data_print, NULL);
				if (ret) 
				{
					GSEC_print(ret, user_data->user_name);
					if (status[1])
					{
						GSEC_print_status(status);
					}
					get_security_error(status, ret);
				}
			}
			else
#endif //SUPERCLIENT
			{
				callRemoteServiceManager(status, sHandle, *user_data, data_print, NULL);
				if (status[1])
				{
					GSEC_print_status(status);
					ret = GsecMsg75;
				}
			}
		}
	}
#ifndef SERVICE_THREAD
	else {
		int local_argc;
		char* local_argv[MAXARGS];
		for (;;) {
			MOVE_CLEAR(status, sizeof(ISC_STATUS_ARRAY));
			/* Clear out user data each time through this loop. */
			MOVE_CLEAR(user_data, sizeof(internal_user_data));
			if (get_line(&local_argc, local_argv, stuff, sizeof(stuff)))
				break;
			if (local_argc > 1) {
				ret = parse_cmd_line(local_argc, local_argv, tdsec);
				if (ret == 1)
				{
					// quit command
					break;
				}
				if (user_data->dba_user_name_entered || 
					user_data->dba_password_entered ||
					user_data->database_name_entered
#ifdef TRUSTED_AUTH
					|| user_data->trusted_auth
#endif
					)
				{
					GSEC_print(GsecMsg92);
					continue;
				}

				databaseName.copyTo(user_data->database_name, sizeof(user_data->database_name));
				user_data->database_name_entered = databaseNameEntered;
				if (ret == 0) {
#ifndef SUPERCLIENT
					if (!useServices)
					{
						ret = SECURITY_exec_line(status, db_handle, 
							user_data, data_print, NULL);
						if (ret) 
						{
							GSEC_print(ret, user_data->user_name);
							if (status[1]) {
								GSEC_print_status(status);
							}
							continue;
						}
					}
					else
#endif //SUPERCLIENT
					{
						callRemoteServiceManager(status, sHandle, *user_data, data_print, NULL);
						if (status[1])
						{
							GSEC_print_status(status);
							continue;
						}
					}
				}
			}
		}
	}
#endif

#ifndef SUPERCLIENT
	if (db_handle) {
		ISC_STATUS_ARRAY loc_status;
		if (isc_detach_database(loc_status, &db_handle)) {
			GSEC_error_redirect(loc_status, GsecMsg93);
		}
	}
#endif //SUPERCLIENT
	if (sHandle)
	{
		ISC_STATUS_ARRAY loc_status;
		detachRemoteServiceManager(loc_status, sHandle);
		if (loc_status[1]) {
			GSEC_print_status(loc_status);
		}
	}

	if (tdsec->tsec_interactive)
		gsec_exit(FINI_OK, tdsec);

	gsec_exit(ret, tdsec);
	return ret;					// silence compiler warning

	}	// try
	catch (const Firebird::LongJump&) {
		/* All calls to gsec_exit(), normal and error exits, wind up here */
		const int exit_code = tdsec->tsec_exit_code;

		tdsec->tsec_service_blk->svc_started();
		tdsec->tsec_throw = false;

		return exit_code;
	}
	catch (const Firebird::Exception& e) {
		// Real exceptions are coming here
		ISC_STATUS *status = tdsec->tsec_status;
		e.stuff_exception(status);

		tdsec->tsec_service_blk->svc_started();
		tdsec->tsec_throw = false;

		GSEC_print_status(status, false);

		return 127;
	}
}


static void data_print(void* arg, const internal_user_data* data, bool first)
{
/**************************************
 *
 *	d a t a _ p r i n t
 *
 **************************************
 *
 * Functional description
 *	print out user data row by row
 *	if first is TRUE print the header then the data
 *
 **************************************/
	tsec* tdsec = tsec::getSpecific();

#ifdef SERVICE_THREAD
#define STUFF_USER(item) SVC_putc(tdsec->tsec_service_blk, item)
#else
#define STUFF_USER(item) fputc(item, stderr)
#endif
	if (tdsec->tsec_service_gsec) {
		int i, len;
		/* Send the username */
		len = strlen(data->user_name);
		STUFF_USER((UCHAR) isc_spb_sec_username);
		STUFF_USER((UCHAR) len);
		STUFF_USER((UCHAR) (len >> 8));
		for (i = 0; i < len; i++)
			STUFF_USER((UCHAR) data->user_name[i]);

		/* Send the first name */
		len = strlen(data->first_name);
		STUFF_USER((UCHAR) isc_spb_sec_firstname);
		STUFF_USER((UCHAR) len);
		STUFF_USER((UCHAR) (len >> 8));
		for (i = 0; i < len; i++)
			STUFF_USER((UCHAR) data->first_name[i]);

		/* Send the middle name */
		len = strlen(data->middle_name);
		STUFF_USER((UCHAR) isc_spb_sec_middlename);
		STUFF_USER((UCHAR) len);
		STUFF_USER((UCHAR) (len >> 8));
		for (i = 0; i < len; i++)
			STUFF_USER((UCHAR) data->middle_name[i]);

		/* Send the last name */
		len = strlen(data->last_name);
		STUFF_USER((UCHAR) isc_spb_sec_lastname);
		STUFF_USER((UCHAR) len);
		STUFF_USER((UCHAR) (len >> 8));
		for (i = 0; i < len; i++)
			STUFF_USER((UCHAR) data->last_name[i]);

		/* Send the uid */
		STUFF_USER((UCHAR) isc_spb_sec_userid);
		STUFF_USER((UCHAR) data->uid);
		STUFF_USER((UCHAR) (data->uid >> 8));
		STUFF_USER((UCHAR) (data->uid >> 16));
		STUFF_USER((UCHAR) (data->uid >> 24));

		/* Send the gid */
		STUFF_USER((UCHAR) isc_spb_sec_groupid);
		STUFF_USER((UCHAR) data->gid);
		STUFF_USER((UCHAR) (data->gid >> 8));
		STUFF_USER((UCHAR) (data->gid >> 16));
		STUFF_USER((UCHAR) (data->gid >> 24));
	}
	else {
		if (first) {
			GSEC_print(GsecMsg26);
			GSEC_print(GsecMsg27);
/* msg26: "    user name                    uid   gid     full name" */
/* msg27: "-------------------------------------------------------------------------------------------" */
		}

		util_output("%-31.31s %5d %5d      %s %s %s\n", data->user_name,
					data->uid, data->gid, data->first_name, data->middle_name,
					data->last_name);
	}
}


static bool get_line(int* argc, SCHAR** argv, TEXT* stuff, size_t maxstuff)
{
/**************************************
 *
 *	g e t _ l i n e
 *
 **************************************
 *
 * Functional description
 *	Read the current line and put its pieces into an argc/argv
 *	structure.   Reads a max of MAXARGS - 1 pieces (argv [0] is
 *	unused), and a max of MAXSTUFF characters, at which point
 *
 **************************************/
	GSEC_print_partial(GsecMsg1);
	*argc = 1;
	TEXT* cursor = stuff;
	int count = (int) maxstuff - 1;
	bool first = true;

/* for each input character, if it's white space (or any non-printable,
   non-newline for that matter), ignore it; if it's a newline, we're
   done; otherwise, put it in the current argument */

	while (*argc < MAXARGS && count > 0) {
		TEXT c = getc(stdin);
		if (c > ' ' && c <= '~') {
			/* note that the first argument gets a '-' appended to the front to fool
			   the switch checker into thinking it came from the command line */

			for (argv[(*argc)++] = cursor; count > 0; count--) {
				if (first) {
					first = false;
					if (c != '?') {
						*cursor++ = '-';
						count--;
					}
				}
				*cursor++ = c;
				c = getc(stdin);
				if (c <= ' ' || c > '~')
					break;
			}
			*cursor++ = '\0';
			count--;
		}
		if (c == '\n')
			break;
		if (c == EOF) {
			if (SYSCALL_INTERRUPTED(errno)) {
				errno = 0;
				continue;
			}
			else
				return true;
		}
	}

	*cursor = '\0';
	return false;
}


static bool get_switches(
							int argc,
							fb_utils::arg_string* argv,
							const in_sw_tab_t* in_sw_table,
							tsec* tdsec, bool* quitflag)
{
/**************************************
 *
 *	g e t _ s w i t c h e s
 *
 **************************************
 *
 * Functional description
 *	Parse the input line arguments, saving
 *	interesting switches in a switch table.
 *
 **************************************/
	TEXT msg[MSG_LENGTH];
	int l;
	char quote;
	SSHORT err_msg_no;

/* look at each argument.   it's either a switch or a parameter.
   parameters must always follow a switch, but not all switches
   need parameters.   this is how, for example, parameters are
   cleared (like a -fname switch followed by no first name
   parameter). */

	internal_user_data* user_data = tdsec->tsec_user_data;
	*quitflag = false;
	USHORT last_sw = IN_SW_GSEC_0;
	tdsec->tsec_sw_version = false;
	for (--argc; argc > 0; argc--) {
		fb_utils::arg_string string = *++argv;
		if (*string == '?')
			user_data->operation = HELP_OPER;
		else if (*string != '-') {
			/* this is not a switch, so it must be a parameter for
			   the previous switch, if any */

			switch (last_sw) {
			case IN_SW_GSEC_ADD:
			case IN_SW_GSEC_DEL:
			case IN_SW_GSEC_DIS:
			case IN_SW_GSEC_MOD:
				quote = ' ';
				for (l = 0; l < 32 && string[l] && string[l] != quote; )
				{
					if (l == 0 && (*string == '\'' || *string == '"'))
					{
						quote = *string++;
						continue;
					}
					user_data->user_name[l] = UPPER(string[l]);
					++l;
				}
				if (l == 32) {
#ifdef SERVICE_THREAD
					GSEC_error(GsecMsg76);
#else
					GSEC_print(GsecMsg76);
#endif
					/* invalid user name (maximum 31 bytes allowed) */
					return false;
				}
				user_data->user_name[l] = '\0';
				user_data->user_name_entered = true;
				break;
			case IN_SW_GSEC_PASSWORD:
				for (l = 0; l < 9 && string[l] && string[l] != ' '; l++)
					user_data->password[l] = string[l];
				if ((l == 9) && !(tdsec->tsec_service_gsec)) {
					GSEC_print(GsecMsg77);
					// warning password maximum 8 significant bytes used
				}
				user_data->password[l] = '\0';
				user_data->password_entered = true;
				break;
			case IN_SW_GSEC_UID:
				user_data->uid = atoi(string);
				user_data->uid_entered = true;
				break;
			case IN_SW_GSEC_GID:
				user_data->gid = atoi(string);
				user_data->gid_entered = true;
				break;
			case IN_SW_GSEC_SYSU:
				strncpy(user_data->sys_user_name, string, sizeof(user_data->sys_user_name));
				user_data->sys_user_entered = true;
				break;
			case IN_SW_GSEC_GROUP:
				strncpy(user_data->group_name, string, sizeof(user_data->group_name));
				user_data->group_name_entered = true;
				break;
			case IN_SW_GSEC_FNAME:
				strncpy(user_data->first_name, string, sizeof(user_data->first_name));
				user_data->first_name_entered = true;
				break;
			case IN_SW_GSEC_MNAME:
				strncpy(user_data->middle_name, string, sizeof(user_data->middle_name));
				user_data->middle_name_entered = true;
				break;
			case IN_SW_GSEC_LNAME:
				strncpy(user_data->last_name, string, sizeof(user_data->last_name));
				user_data->last_name_entered = true;
				break;
			case IN_SW_GSEC_DATABASE:
				strncpy(user_data->database_name, string, sizeof(user_data->database_name));
				user_data->database_name_entered = true;
				break;
			case IN_SW_GSEC_DBA_USER_NAME:
				strncpy(user_data->dba_user_name, string, sizeof(user_data->dba_user_name));
				user_data->dba_user_name_entered = true;
				break;
			case IN_SW_GSEC_DBA_PASSWORD:
				strncpy(user_data->dba_password, fb_utils::get_passwd(string), sizeof(user_data->dba_password));
				user_data->dba_password_entered = true;
				break;
			case IN_SW_GSEC_SQL_ROLE_NAME:
				strncpy(user_data->sql_role_name, string, sizeof(user_data->sql_role_name));
				user_data->sql_role_name_entered = true;
				break;
#ifdef TRUSTED_SERVICES
			case IN_SW_GSEC_DBA_TRUST_USER:
				strncpy(user_data->dba_trust_user_name, string, sizeof(user_data->dba_trust_user_name));
				user_data->dba_trust_user_name_entered = true;
				break;
#endif
			case IN_SW_GSEC_Z:
			case IN_SW_GSEC_0:
#ifdef SERVICE_THREAD
				GSEC_error(GsecMsg29);
#else
				GSEC_print(GsecMsg29);
#endif
				/* gsec - invalid parameter, no switch defined */
				return false;
			}
			last_sw = IN_SW_GSEC_0;
		}
		else {
			/* iterate through the switch table, looking for matches */

			USHORT in_sw = IN_SW_GSEC_0;
			{ // scope
			const TEXT* q;
			for (const in_sw_tab_t* in_sw_tab = in_sw_table;
				q = in_sw_tab->in_sw_name; in_sw_tab++)
			{
				const TEXT* p = string + 1;

				/* handle orphaned hyphen case */

				if (!*p--)
					break;

				/* compare switch to switch name in table */

				l = 0;
				while (*p) {
					if (!*++p) {
						if (l >= in_sw_tab->in_sw_min_length)
							in_sw = in_sw_tab->in_sw;
						else
							in_sw = IN_SW_GSEC_AMBIG;
					}
					if (UPPER(*p) != *q++)
						break;
					l++;
				}

				/* end of input means we got a match.  stop looking */

				if (!*p)
					break;
			}
			} // scope

			/* this checks to make sure that the switch is not a duplicate.   if
			   it is a duplicate, it's an error.   if it's not a duplicate, the
			   appropriate specified flag is set (to later check for duplicates),
			   and the applicable parameter value is set to its null value, in
			   case the user really wants to remove an existing parameter. */

			switch (in_sw) {
			case IN_SW_GSEC_ADD:
			case IN_SW_GSEC_DEL:
			case IN_SW_GSEC_DIS:
			case IN_SW_GSEC_MOD:
			case IN_SW_GSEC_QUIT:
			case IN_SW_GSEC_HELP:
				if (user_data->operation) {
					GSEC_error(GsecMsg30);
					/* gsec - operation already specified */
					return false;
				}
				switch (in_sw) {
				case IN_SW_GSEC_ADD:
					user_data->operation = ADD_OPER;
					break;
				case IN_SW_GSEC_DEL:
					user_data->operation = DEL_OPER;
					break;
				case IN_SW_GSEC_DIS:
					user_data->operation = DIS_OPER;
					break;
				case IN_SW_GSEC_MOD:
					user_data->operation = MOD_OPER;
					break;
				case IN_SW_GSEC_QUIT:
					user_data->operation = QUIT_OPER;
					*quitflag = true;
					break;
				case IN_SW_GSEC_HELP:
					user_data->operation = HELP_OPER;
					break;
				}
				user_data->user_name[0] = '\0';
				tdsec->tsec_interactive = false;
				break;
			case IN_SW_GSEC_PASSWORD:
			case IN_SW_GSEC_UID:
			case IN_SW_GSEC_GID:
			case IN_SW_GSEC_SYSU:
			case IN_SW_GSEC_GROUP:
			case IN_SW_GSEC_FNAME:
			case IN_SW_GSEC_MNAME:
			case IN_SW_GSEC_LNAME:
			case IN_SW_GSEC_DATABASE:
#ifdef TRUSTED_SERVICES
			case IN_SW_GSEC_DBA_TRUST_USER:
#endif
			case IN_SW_GSEC_DBA_USER_NAME:
			case IN_SW_GSEC_DBA_PASSWORD:
			case IN_SW_GSEC_SQL_ROLE_NAME:
				err_msg_no = 0;
				switch (in_sw) {
				case IN_SW_GSEC_PASSWORD:
					if (user_data->password_specified) {
						err_msg_no = GsecMsg31;
						break;
					}
					user_data->password_specified = true;
					user_data->password[0] = '\0';
					break;
				case IN_SW_GSEC_UID:
					if (user_data->uid_specified) {
						err_msg_no = GsecMsg32;
						break;
					}
					user_data->uid_specified = true;
					user_data->uid = 0;
					break;
				case IN_SW_GSEC_GID:
					if (user_data->gid_specified) {
						err_msg_no = GsecMsg33;
						break;
					}
					user_data->gid_specified = true;
					user_data->gid = 0;
					break;
				case IN_SW_GSEC_SYSU:
					if (user_data->sys_user_specified) {
						err_msg_no = GsecMsg34;
						break;
					}
					user_data->sys_user_specified = true;
					user_data->sys_user_name[0] = '\0';
					break;
				case IN_SW_GSEC_GROUP:
					if (user_data->group_name_specified) {
						err_msg_no = GsecMsg35;
						break;
					}
					user_data->group_name_specified = true;
					user_data->group_name[0] = '\0';
					break;
				case IN_SW_GSEC_FNAME:
					if (user_data->first_name_specified) {
						err_msg_no = GsecMsg36;
						break;
					}
					user_data->first_name_specified = true;
					user_data->first_name[0] = '\0';
					break;
				case IN_SW_GSEC_MNAME:
					if (user_data->middle_name_specified) {
						err_msg_no = GsecMsg37;
						break;
					}
					user_data->middle_name_specified = true;
					user_data->middle_name[0] = '\0';
					break;
				case IN_SW_GSEC_LNAME:
					if (user_data->last_name_specified) {
						err_msg_no = GsecMsg38;
						break;
					}
					user_data->last_name_specified = true;
					user_data->last_name[0] = '\0';
					break;
				case IN_SW_GSEC_DATABASE:
					if (user_data->database_name_specified) {
						err_msg_no = GsecMsg78;
						break;
					}
					user_data->database_name_specified = true;
					user_data->database_name[0] = '\0';
					break;
				case IN_SW_GSEC_DBA_USER_NAME:
					if (user_data->dba_user_name_specified) {
						err_msg_no = GsecMsg79;
						break;
					}
					user_data->dba_user_name_specified = true;
					user_data->dba_user_name[0] = '\0';
					break;
#ifdef TRUSTED_SERVICES
				case IN_SW_GSEC_DBA_TRUST_USER:
					if (user_data->dba_trust_user_name_specified) {
						err_msg_no = GsecMsg79;
						break;
					}
					user_data->dba_trust_user_name_specified = true;
					user_data->dba_trust_user_name[0] = '\0';
					break;
#endif
				case IN_SW_GSEC_DBA_PASSWORD:
					if (user_data->dba_password_specified) {
						err_msg_no = GsecMsg80;
						break;
					}
					user_data->dba_password_specified = true;
					user_data->dba_password[0] = '\0';
					break;
				case IN_SW_GSEC_SQL_ROLE_NAME:
					if (user_data->sql_role_name_specified) {
						err_msg_no = GsecMsg81;
						break;
					}
					user_data->sql_role_name_specified = true;
					user_data->sql_role_name[0] = '\0';
					break;
				}
				if (err_msg_no) {
					GSEC_error(err_msg_no);
					return false;
				}
				break;
			case IN_SW_GSEC_Z:
				if (!tdsec->tsec_sw_version) {
					msg_get(GsecMsg39, msg);
					util_output("%s %s\n", msg, GDS_VERSION);
				}
				tdsec->tsec_sw_version = true;
				break;
#ifdef TRUSTED_AUTH
			case IN_SW_GSEC_TRUSTED_AUTH:
				user_data->trusted_auth = true;
				break;
#endif
			case IN_SW_GSEC_0:
#ifdef SERVICE_THREAD
				GSEC_error(GsecMsg40);
#else
				GSEC_print(GsecMsg40);
#endif
				/* gsec - invalid switch specified */
				return false;
			case IN_SW_GSEC_AMBIG:
#ifdef SERVICE_THREAD
				GSEC_error(GsecMsg41);
#else
				GSEC_print(GsecMsg41);
#endif
				/* gsec - ambiguous switch specified */
				return false;
			}
			last_sw = in_sw;
		}

		/* make sure that the current suite of switches and parameters
		   is valid, and if not, indicate why not */

		if (user_data->uid_entered || user_data->gid_entered ||
			user_data->sys_user_entered || user_data->group_name_entered ||
			user_data->password_entered || user_data->first_name_entered ||
			user_data->middle_name_entered || user_data->last_name_entered)
		{
			switch (user_data->operation) {
			case 0:
				GSEC_error(GsecMsg42);
				/* gsec - no operation specified for parameters */
				return false;
			case ADD_OPER:
			case MOD_OPER:
				/* any parameter is ok for these operation states */
				break;
			case DEL_OPER:
			case DIS_OPER:
			case QUIT_OPER:
			case HELP_OPER:
				GSEC_error(GsecMsg43);
				/* gsec - no parameters allowed for this operation */
				return false;
			}
		}

		if (*quitflag)
			break;
	}

	return true;
}


static void printhelp(void)
{
/**************************************
 *
 *	p r i n t h e l p
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

	util_output("\n%s", "   ");
	GSEC_print(GsecMsg45);
/* gsec utility - maintains user password database */

	util_output("\n%s", "   ");
	GSEC_print(GsecMsg46);
	util_output("%s", "     ");
	GSEC_print_partial(GsecMsg2);
	GSEC_print_partial(GsecMsg82);
	GSEC_print(GsecMsg47);
/* gsec [ <options> ... ] -<command> [ <parameter> ... ] */

	util_output("\n%s", "   ");
	GSEC_print(GsecMsg48);
/* interactive usage: */

	util_output("%s", "     ");
	GSEC_print_partial(GsecMsg2);
	GSEC_print(GsecMsg82);
/* gsec [ <options> ... ] */

	util_output("%s", "     ");
	GSEC_print_partial(GsecMsg1);
	util_output("\n%s", "     ");
	GSEC_print(GsecMsg47);
/* GSEC> <command> [ <parameter> ... ] */

	util_output("\n%s", "   ");
	GSEC_print(GsecMsg83);
/* available options: */

	util_output("%s", "     ");
	GSEC_print(GsecMsg84);
/* -user <database administrator name> */

	util_output("%s", "     ");
	GSEC_print(GsecMsg85);
/* -password <database administrator password> */

	util_output("%s", "     ");
	GSEC_print(GsecMsg86);
/* -role <database administrator SQL role name> */

	util_output("%s", "     ");
	GSEC_print(GsecMsg91);
/* -trusted (use trusted authentication) */

	util_output("%s", "     ");
	GSEC_print(GsecMsg87);
/* -database <database to manage> */

	util_output("%s", "     ");
	GSEC_print(GsecMsg88);
/* -z */

	util_output("\n%s", "   ");
	GSEC_print(GsecMsg49);
/* available commands: */

	util_output("%s", "     ");
	GSEC_print(GsecMsg50);
/* adding a new user: */

	util_output("%s", "       ");
	GSEC_print(GsecMsg51);
/* add <name> [ <parameter> ... ] */

	util_output("%s", "     ");
	GSEC_print(GsecMsg52);
/* deleting a current user: */

	util_output("%s", "       ");
	GSEC_print(GsecMsg53);
/* delete <name> */

	util_output("%s", "     ");
	GSEC_print(GsecMsg54);
/* displaying all users: */

	util_output("%s", "       ");
	GSEC_print(GsecMsg55);
/* display */

	util_output("%s", "     ");
	GSEC_print(GsecMsg56);
/* displaying one user: */

	util_output("%s", "       ");
	GSEC_print(GsecMsg57);
/* display <name> */

	util_output("%s", "     ");
	GSEC_print(GsecMsg58);
/* modifying a user's parameters: */

	util_output("%s", "       ");
	GSEC_print(GsecMsg59);
/* modify <name> <parameter> [ <parameter> ... ] */

	util_output("%s", "     ");
	GSEC_print(GsecMsg60);
/* help: */

	util_output("%s", "       ");
	GSEC_print(GsecMsg61);
/* ? (interactive only) */

	util_output("%s", "       ");
	GSEC_print(GsecMsg62);
/* help */

	util_output("%s", "     ");
	GSEC_print(GsecMsg89);
/* displaying version number: */

	util_output("%s", "       ");
	GSEC_print(GsecMsg90);
/* z (interactive only) */

	util_output("%s", "     ");
	GSEC_print(GsecMsg63);
/* quit interactive session: */

	util_output("%s", "       ");
	GSEC_print(GsecMsg64);
/* quit (interactive only) */

	util_output("\n%s", "   ");
	GSEC_print(GsecMsg65);
/* available parameters: */

	util_output("%s", "     ");
	GSEC_print(GsecMsg66);
/* -pw <password> */

	util_output("%s", "     ");
	GSEC_print(GsecMsg67);
/* -uid <uid> */

	util_output("%s", "     ");
	GSEC_print(GsecMsg68);
/* -gid <gid> */

	util_output("%s", "     ");
	GSEC_print(GsecMsg71);
/* -fname <firstname> */

	util_output("%s", "     ");
	GSEC_print(GsecMsg72);
/* -mname <middlename> */

	util_output("%s", "     ");
	GSEC_print(GsecMsg73);
/* -lname <lastname> */
	util_output("\n", NULL);
}


static SSHORT parse_cmd_line(int argc, fb_utils::arg_string* argv, tsec* tdsec)
{
/**************************************
 *
 *	p a r s e _ c m d _ l i n e
 *
 **************************************
 *
 * Functional description
 *	Read the command line
 *	returns 0 on normal completion, 
 *	   1 if user chooses to quit
 *	   -1 on error or if user asks for help
 *
 **************************************/
	bool quitflag = false;
	internal_user_data* user_data = tdsec->tsec_user_data;
	memset(user_data, 0, sizeof(internal_user_data));

/* Call a subroutine to process the input line. */

	SSHORT ret = 0;
	if (!get_switches(argc, argv, gsec_in_sw_table, tdsec, &quitflag)) {
#ifdef SERVICE_THREAD
		GSEC_error(GsecMsg16);
#else
		GSEC_print(GsecMsg16);
#endif
		/* gsec - error in switch specifications */
		ret = -1;
	}
	else if (user_data->operation)
		if (user_data->operation == HELP_OPER) {
			printhelp();
			ret = -1;
		}
		else if (user_data->operation != DIS_OPER &&
				 user_data->operation != QUIT_OPER &&
				 !user_data->user_name_entered)
		{
			GSEC_error(GsecMsg18);
			/* gsec - no user name specified */
			ret = -1;
		}

	if (quitflag)
		ret = 1;

	if (tdsec->tsec_sw_version)
		ret = -1;

	return ret;
}

void GSEC_print_status(const ISC_STATUS* status_vector, bool exitOnError)
{
/**************************************
 *
 *	U T I L _ p r i n t _ s t a t u s
 *
 **************************************
 *
 * Functional description
 *	Print error message. Use fb_interpret
 *	to allow redirecting output.
 *
 **************************************/
	if (status_vector) {
		const ISC_STATUS* vector = status_vector;
#ifdef SERVICE_THREAD
		tsec* tdsec = tsec::getSpecific();
		ISC_STATUS* status = tdsec->tsec_service_blk->svc_status;
		if (status != status_vector) {
		    int i = 0;
			while (*status && (++i < ISC_STATUS_LENGTH))
				status++;
			for (int j = 0; status_vector[j] && (i < ISC_STATUS_LENGTH); j++, i++)
				*status++ = status_vector[j];
		}
#else
		SCHAR s[1024];
		while (fb_interpret(s, sizeof(s), &vector)) 
		{
			TRANSLATE_CP(s);
			const char* nl = (s[0] ? s[strlen(s) - 1] != '\n' : true) ? "\n" : "";
			int exitCode = util_print("%s%s", s, nl);
			if (exitOnError && exitCode != 0) 
			{
				gsec_exit(exitCode, tsec::getSpecific());
			}
		}
#endif
	}
}

static int vutil_print(const SCHAR* format, va_list arglist)
{
/**************************************
 *
 *	v u t i l _ p r i n t
 *
 **************************************
 *
 * Functional description
 *	Platform independent output routine.
 *  Varargs function.
 *
 **************************************/
	tsec* tdsec = tsec::getSpecific();

	Firebird::string buf;
	buf.vprintf(format, arglist);
	
	return tdsec->tsec_output_proc(tdsec->tsec_output_data, (UCHAR*)(buf.c_str()));
}

static void util_output(const SCHAR* format, ...)
{
/**************************************
 *
 *	u t i l _ o u t p u t
 *
 **************************************
 *
 * Functional description
 *	Platform independent output routine.
 *  Exit on output error
 *
 **************************************/
	va_list arglist;
	va_start(arglist, format);
	int exit_code = vutil_print(format, arglist);
	va_end(arglist);

	if (exit_code != 0) 
	{
		gsec_exit(exit_code, tsec::getSpecific());
	}
}

static int util_print(const SCHAR* format, ...)
{
/**************************************
 *
 *	u t i l _ o u t p u t
 *
 **************************************
 *
 * Functional description
 *	Platform independent output routine.
 *
 **************************************/
	va_list arglist;
	va_start(arglist, format);
	int exit_code = vutil_print(format, arglist);
	va_end(arglist);

	return exit_code;
}

void GSEC_error_redirect(const ISC_STATUS* status_vector,
						 USHORT errcode)
{
/**************************************
 *
 *	G S E C _ e r r o r _ r e d i r e c t
 *
 **************************************
 *
 * Functional description
 *	Issue error message. Output messages then abort.
 *
 **************************************/

	GSEC_print_status(status_vector);
	GSEC_error(errcode);
}

void GSEC_error(USHORT errcode)
{
/**************************************
 *
 *	 G S E C _ e r r o r
 *
 **************************************
 *
 * Functional description
 *	Format and print an error message, then punt.
 *
 **************************************/
	static const SafeArg dummy;

	tsec* tdsec = tsec::getSpecific();
#ifdef SERVICE_THREAD
	ISC_STATUS* status = tdsec->tsec_service_blk->svc_status;

	CMD_UTIL_put_svc_status(status, GSEC_MSG_FAC, errcode, dummy);
	tdsec->tsec_service_blk->svc_started();
#endif

	GSEC_print(errcode);
	gsec_exit(FINI_ERROR, tdsec);
}

void GSEC_print(USHORT number,
				const char* str)
{
/**************************************
 *
 *	G S E C _ p r i n t
 *
 **************************************
 *
 * Functional description
 *	Retrieve a message from the error file, format it, and print it.
 *
 **************************************/
	TEXT buffer[256];

	SafeArg arg;
	if (str)
		arg << str;

	fb_msg_format(0, GSEC_MSG_FAC, number, sizeof(buffer), buffer, arg);
	TRANSLATE_CP(buffer);
	util_output("%s\n", buffer);
}

void GSEC_print_partial(USHORT number)
{
/**************************************
 *
 *	G S E C _ p r i n t _ p a r t i a l
 *
 **************************************
 *
 * Functional description
 *	Retrieve a message from the error file, format it, and print it.
 *
 **************************************/
	static const SafeArg dummy;
	TEXT buffer[256];

	fb_msg_format(0, GSEC_MSG_FAC, number, sizeof(buffer), buffer, dummy);
	TRANSLATE_CP(buffer);
	util_output("%s ", buffer);
}


inline void msg_get(USHORT number, TEXT* msg)
{
/**************************************
 *
 *	m s g _ g e t
 *
 **************************************
 *
 * Functional description
 *	Retrieve a message from the error file
 *
 **************************************/

	static const SafeArg dummy;
	fb_msg_format(NULL, GSEC_MSG_FAC, number, MSG_LENGTH, msg, dummy);
}


static void insert_error(ISC_STATUS* status, ISC_STATUS isc_err)
{
/**************************************
 *
 *      i n s e r t _ e r r o r
 *
 **************************************
 *
 * Functional description
 *
 *	Adds isc error code to status vector
 **************************************/
	if (status[1])
	{
		memmove(&status[2], &status[0], sizeof(ISC_STATUS_ARRAY) - 2 * sizeof(ISC_STATUS));
	}
	else
	{
		status[2] = isc_arg_end;
	}
	status[0] = isc_arg_gds;
	status[1] = isc_err;
}


static void get_security_error(ISC_STATUS* status, int gsec_err)
{
/**************************************
 *
 *      g e t _ s e c u r i t y _ e r r o r
 *
 **************************************
 *
 * Functional description
 *
 *    Converts the gsec error code to an isc
 *    error code and adds it to the status vector
 **************************************/


	switch (gsec_err) {
	case GsecMsg19:			/* gsec - add record error */
		insert_error(status, isc_error_adding_sec_record);
		return;

	case GsecMsg20:			/* gsec - modify record error */
		insert_error(status, isc_error_modifying_sec_record);
		return;

	case GsecMsg21:			/* gsec - find/modify record error */
		insert_error(status, isc_error_modifying_sec_record);
		return;

	case GsecMsg22:			/* gsec - record not found for user: */
		insert_error(status, isc_usrname_not_found);
		return;

	case GsecMsg23:			/* gsec - delete record error */
		insert_error(status, isc_error_deleting_sec_record);
		return;

	case GsecMsg24:			/* gsec - find/delete record error */
		insert_error(status, isc_error_deleting_sec_record);
		return;

	case GsecMsg75:			/* gsec error */
		insert_error(status, isc_error_updating_sec_db);
		return;

	default:
		return;
	}
}
