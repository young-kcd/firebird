/*
 *
 *	PROGRAM:	Security data base manager
 *	MODULE:		gsec.c
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
#include "../jrd/ib_stdio.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>
#include "../jrd/common.h"
#include "../jrd/gds.h"
#include "../jrd/gds_proto.h"
#include "../jrd/msg_encode.h"
#include "../jrd/jrd.h"
#include "../utilities/gsec/gsec.h"
#include "../jrd/jrd_pwd.h"
#include "../jrd/license.h"
#include "../jrd/svc_proto.h"
#include "../utilities/gsec/secur_proto.h"
#include "../utilities/gsec/gsecswi.h"
#include "../utilities/common/cmd_util_proto.h"
#ifdef WIN_NT
#include <windows.h>
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#if defined (WIN95)
static BOOLEAN fAnsiCP = FALSE;
#define TRANSLATE_CP(a) if (!fAnsiCP) CharToOem(a, a)
#else
#define TRANSLATE_CP(a)
#endif

#define MAXARGS		20			/* max number of args allowed on command line */
#define MAXSTUFF	1000		/* longest interactive command line */

#ifndef SUPERSERVER
struct tsec *gdsec;
#endif

int UTIL_gsec(int, char **, OUTPUTPROC, SLONG);
/* Output reporting utilities */
void UTIL_print_status(ISC_STATUS *);
void UTIL_error_redirect(ISC_STATUS *, USHORT, TEXT *, TEXT *);
void UTIL_error(USHORT, TEXT *, TEXT *, TEXT *, TEXT *, TEXT *);
void UTIL_print(USHORT, TEXT *, TEXT *, TEXT *, TEXT *, TEXT *);
void UTIL_print_partial(USHORT, TEXT *, TEXT *, TEXT *, TEXT *, TEXT *);
static void util_output(const SCHAR *, ...);

static void data_print(void *, USER_DATA, BOOLEAN);
static int get_line(int *, SCHAR **, TEXT *, TSEC);
static BOOLEAN get_switches(int, TEXT **, IN_SW_TAB, TSEC, BOOLEAN *);
static SSHORT parse_cmd_line(int, TEXT **, TSEC);
static void printhelp(void);
int output_svc(SLONG, UCHAR *);
#ifndef SUPERSERVER
static int output_main(SLONG, UCHAR *);
#endif

#ifdef SUPERSERVER
int main_gsec( SVC service)
{
/**********************************************
 *
 *    m a i n _ g s e c
 *
 **********************************************
 * Functional Description:
 *   Entrypoint for GSEC via the services manager
 **********************************************/
	int exit_code;

	exit_code = UTIL_gsec(service->svc_argc, service->svc_argv,
						  output_svc, (SLONG) service);

	service->svc_handle = 0;
	if (service->svc_service->in_use != NULL)
		*(service->svc_service->in_use) = FALSE;

/* Mark service thread as finished. */
/* If service is detached, cleanup memory being used by service. */
	SVC_finish(service, SVC_finished);

	return exit_code;
}


int output_svc( SLONG output_data, UCHAR * output_buf)
{
/**************************************
 *
 *	o u t p u t _ s v c
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

#else

int CLIB_ROUTINE main( int argc, char *argv[])
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
	UTIL_gsec(argc, argv, output_main, (SLONG) NULL);
	return 0;
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

#endif /* SUPERSERVER */

int UTIL_gsec(
			  int argc,
			  char *argv[], OUTPUTPROC output_proc, SLONG output_data)
{
/**************************************
 *
 *	U T I L _ g s e c
 *
 **************************************
 *
 * Functional description
 *	If there is no command line, prompt for one, read it
 *	and make an artificial argc/argv.   Otherwise, pass
 *	the specified argc/argv to SECURITY_exec_line (see below).
 *
 **************************************/
	ISC_STATUS *status;
	FRBRD *db_handle = NULL;		/* user info database handle */
	TEXT user_info_name[MAXPATHLEN], *u;	/* user info database name */
	SLONG redir_in, redir_out, redir_err;
	int local_argc;
	SCHAR *local_argv[MAXARGS];
	TEXT stuff[MAXSTUFF];		/* a place to put stuff */
	SSHORT ret;
	char dpb_buffer[256], *dpb, *p;
	short dpb_length;
	TSEC tdsec;
	USER_DATA user_data;
	JMP_BUF env;

#ifdef VMS
	int argc;
	argc = VMS_parse(&argv, argc);
#endif

	tdsec = (struct tsec *) gds__alloc(sizeof(*tdsec));
/* NOMEM: return error, FREE: during function exit in the SETJMP */
	if (tdsec == NULL) {
		EXIT(FINI_ERROR);
	}

	SET_THREAD_DATA;
	SVC_PUTSPECIFIC_DATA;
	memset((void *) tdsec, 0, sizeof(*tdsec));

	tdsec->tsec_user_data =
		(struct user_data *) gds__alloc(sizeof(*user_data));
/* NOMEM: return error, FREE: during function exit in the SETJMP */
	if (tdsec->tsec_user_data == NULL) {
		EXIT(FINI_ERROR);
	}

	memset((void *) tdsec->tsec_user_data, 0, sizeof(*user_data));

	try {

/* Perform some special handling when run as an Interbase service.  The
   first switch can be "-svc" (lower case!) or it can be "-svc_re" followed
   by 3 file descriptors to use in re-directing ib_stdin, ib_stdout, and ib_stderr. */

	tdsec->tsec_env = &env;
	tdsec->tsec_output_proc = output_proc;
	tdsec->tsec_output_data = output_data;
	tdsec->tsec_interactive = TRUE;
	tdsec->tsec_service_gsec = FALSE;
	tdsec->tsec_service_thd = FALSE;
	tdsec->tsec_service_blk = NULL;
	tdsec->tsec_status = tdsec->tsec_status_vector;
	user_data = tdsec->tsec_user_data;

	if (argc > 1 && !strcmp(argv[1], "-svc")) {
		tdsec->tsec_service_gsec = TRUE;
		argv++;
		argc--;
	}
#ifdef SUPERSERVER
	else if (argc > 1 && !strcmp(argv[1], "-svc_thd")) {
		tdsec->tsec_service_gsec = TRUE;
		tdsec->tsec_service_thd = TRUE;
		tdsec->tsec_service_blk = (SVC) output_data;
		tdsec->tsec_status = tdsec->tsec_service_blk->svc_status;
		argv++;
		argc--;
	}
#endif
	else if (argc > 4 && !strcmp(argv[1], "-svc_re")) {
		tdsec->tsec_service_gsec = TRUE;
		redir_in = atol(argv[2]);
		redir_out = atol(argv[3]);
		redir_err = atol(argv[4]);
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

	status = tdsec->tsec_status;
	ret = parse_cmd_line(argc, argv, tdsec);

	if (user_data->database_entered)
		u = user_data->database_name;
	else {
#ifdef SUPERSERVER
/* there is no need to call the services manager to get this information */
		SecurityDatabase::getPath(user_info_name);
#else
		SECURITY_get_db_path(NULL, user_info_name);
#endif
		u = user_info_name;
	}

	dpb = dpb_buffer;
	*dpb++ = isc_dpb_version1;

	if (user_data->dba_user_name_entered) {
		*dpb++ = isc_dpb_user_name;
		*dpb++ = strlen(user_data->dba_user_name);
		for (p = user_data->dba_user_name; *p;)
			*dpb++ = *p++;
	}

	if (user_data->dba_password_entered) {
		if (tdsec->tsec_service_thd)
			*dpb++ = isc_dpb_password_enc;
		else
			*dpb++ = isc_dpb_password;
		*dpb++ = strlen(user_data->dba_password);
		for (p = user_data->dba_password; *p;)
			*dpb++ = *p++;
	}

	if (user_data->sql_role_name_entered) {
		*dpb++ = isc_dpb_sql_role_name;
		*dpb++ = strlen(user_data->sql_role_name);
		for (p = user_data->sql_role_name; *p;)
			*dpb++ = *p++;
	}

	dpb_length = dpb - dpb_buffer;

	if (isc_attach_database(status, 0, u, &db_handle, dpb_length, dpb_buffer))
		UTIL_error_redirect(status, GsecMsg15, NULL, NULL);


	if (!tdsec->tsec_interactive) {
		if (ret == 0) {
#ifdef SUPERSERVER
			/* Signal the start of the service here ONLY if we are displaying users
			 * since the number of users may exceed the service buffer.  This
			 * will cause the service to wait for the client to request data.  However,
			 * if the server is not signaled, then the client can never request anything */
			if (user_data->operation == DIS_OPER)
				SVC_STARTED(tdsec->tsec_service_blk);
#endif
			ret = SECURITY_exec_line(status, db_handle, user_data,
									 data_print, NULL);
			if (ret) {
				UTIL_print(ret, user_data->user_name, NULL, NULL, NULL, NULL);
				if (status[1])
					UTIL_print_status(status);
			}
		}
	}
	else {
		for (;;) {
			/* Clear out user data each time through this loop. */
			MOVE_CLEAR(tdsec->tsec_user_data, sizeof(struct user_data));
			if (get_line(&local_argc, local_argv, stuff, tdsec))
				break;
			if (local_argc > 1) {
				ret = parse_cmd_line(local_argc, local_argv, tdsec);
				if (ret == 1)
					break;
				if (ret == 0) {
					ret = SECURITY_exec_line(status, db_handle, user_data,
											 data_print, NULL);
					if (ret) {
						UTIL_print(ret, user_data->user_name, NULL, NULL,
								   NULL, NULL);
						if (status[1])
							UTIL_print_status(status);
						break;
					}
				}
			}
		}
	}

	if (db_handle) {
		ISC_STATUS_ARRAY loc_status;
		if (isc_detach_database(loc_status, &db_handle))
			UTIL_error_redirect(loc_status, 0, NULL, NULL);
	}
	EXIT(FINI_OK);
	return 0;					// silence compiler warning

	}	// try
	catch (const std::exception&) {
		/* All calls to EXIT(), normal and error exits, wind up here */
		SVC_STARTED(tdsec->tsec_service_blk);
		tdsec->tsec_env = NULL;
		const int exit_code = tdsec->tsec_exit_code;

		if (tdsec->tsec_user_data != NULL)
			gds__free((SLONG *) tdsec->tsec_user_data);

		if (tdsec != NULL)
			gds__free((SLONG *) tdsec);

		/* All returns occur from this point - even normal returns */
		return exit_code;
	}
}


static void data_print( void *arg, USER_DATA data, BOOLEAN first)
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
#ifdef SUPERSERVER
	TSEC tdsec;
	int i, len;
	tdsec = GET_THREAD_DATA;

/* Send the username */
	len = strlen(data->user_name);
	SVC_putc(tdsec->tsec_service_blk, (UCHAR) isc_spb_sec_username);
	SVC_putc(tdsec->tsec_service_blk, (UCHAR) len);
	SVC_putc(tdsec->tsec_service_blk, (UCHAR) (len >> 8));
	for (i = 0; i < len; i++)
		SVC_putc(tdsec->tsec_service_blk, (UCHAR) data->user_name[i]);

/* Send the first name */
	len = strlen(data->first_name);
	SVC_putc(tdsec->tsec_service_blk, (UCHAR) isc_spb_sec_firstname);
	SVC_putc(tdsec->tsec_service_blk, (UCHAR) len);
	SVC_putc(tdsec->tsec_service_blk, (UCHAR) (len >> 8));
	for (i = 0; i < len; i++)
		SVC_putc(tdsec->tsec_service_blk, (UCHAR) data->first_name[i]);

/* Send the middle name */
	len = strlen(data->middle_name);
	SVC_putc(tdsec->tsec_service_blk, (UCHAR) isc_spb_sec_middlename);
	SVC_putc(tdsec->tsec_service_blk, (UCHAR) len);
	SVC_putc(tdsec->tsec_service_blk, (UCHAR) (len >> 8));
	for (i = 0; i < len; i++)
		SVC_putc(tdsec->tsec_service_blk, (UCHAR) data->middle_name[i]);

/* Send the last name */
	len = strlen(data->last_name);
	SVC_putc(tdsec->tsec_service_blk, (UCHAR) isc_spb_sec_lastname);
	SVC_putc(tdsec->tsec_service_blk, (UCHAR) len);
	SVC_putc(tdsec->tsec_service_blk, (UCHAR) (len >> 8));
	for (i = 0; i < len; i++)
		SVC_putc(tdsec->tsec_service_blk, (UCHAR) data->last_name[i]);

/* Send the uid */
	SVC_putc(tdsec->tsec_service_blk, (UCHAR) isc_spb_sec_userid);
	SVC_putc(tdsec->tsec_service_blk, (UCHAR) data->uid);
	SVC_putc(tdsec->tsec_service_blk, (UCHAR) (data->uid >> 8));
	SVC_putc(tdsec->tsec_service_blk, (UCHAR) (data->uid >> 16));
	SVC_putc(tdsec->tsec_service_blk, (UCHAR) (data->uid >> 24));

/* Send the gid */
	SVC_putc(tdsec->tsec_service_blk, (UCHAR) isc_spb_sec_groupid);
	SVC_putc(tdsec->tsec_service_blk, (UCHAR) data->gid);
	SVC_putc(tdsec->tsec_service_blk, (UCHAR) (data->gid >> 8));
	SVC_putc(tdsec->tsec_service_blk, (UCHAR) (data->gid >> 16));
	SVC_putc(tdsec->tsec_service_blk, (UCHAR) (data->gid >> 24));

#else
	if (first) {
		UTIL_print(GsecMsg26, NULL, NULL, NULL, NULL, NULL);
		UTIL_print(GsecMsg27, NULL, NULL, NULL, NULL, NULL);
/* msg26: "    user name                    uid   gid     full name" */
/* msg27: "-------------------------------------------------------------------------------------------" */
	}

	util_output("%-31.31s %5d %5d      %s %s %s\n", data->user_name,
				data->uid, data->gid, data->first_name, data->middle_name,
				data->last_name);
#endif
}


static int get_line( int *argc, SCHAR ** argv, TEXT * stuff, TSEC tdsec)
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
	USHORT count;
	TEXT *cursor, c;
	BOOLEAN first;

	UTIL_print_partial(GsecMsg1, NULL, NULL, NULL, NULL, NULL);
	*argc = 1;
	cursor = stuff;
	count = MAXSTUFF - 1;
	first = TRUE;

/* for each input character, if it's white space (or any non-printable,
   non-newline for that matter), ignore it; if it's a newline, we're
   done; otherwise, put it in the current argument */

	while (*argc < MAXARGS && count > 0) {
		c = ib_getc(ib_stdin);
		if (c > ' ' && c <= '~') {
			/* note that the first argument gets a '-' appended to the front to fool
			   the switch checker into thinking it came from the command line */

			for (argv[(*argc)++] = cursor; count > 0; count--) {
				if (first) {
					first = FALSE;
					if (c != '?') {
						*cursor++ = '-';
						count--;
					}
				}
				*cursor++ = c;
				c = ib_getc(ib_stdin);
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
				return TRUE;
		}
	}

	*cursor = '\0';
	return FALSE;
}


static BOOLEAN get_switches(
							int argc,
							TEXT ** argv,
							IN_SW_TAB in_sw_table,
							TSEC tdsec, BOOLEAN * quitflag)
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
	TEXT *p, *q, *string, msg[MSG_LENGTH];
	IN_SW_TAB in_sw_tab;
	USHORT in_sw;
	USHORT last_sw;
	int l;
	SSHORT err_msg_no;
	USER_DATA user_data;

/* look at each argument.   it's either a switch or a parameter.
   parameters must always follow a switch, but not all switches
   need parameters.   this is how, for example, parameters are
   cleared (like a -fname switch followed by no first name
   parameter). */

	user_data = tdsec->tsec_user_data;
	*quitflag = FALSE;
	last_sw = IN_SW_GSEC_0;
	tdsec->tsec_sw_version = FALSE;
	for (--argc; argc > 0; argc--) {
		string = *++argv;
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
				for (l = 0; l < 32 && string[l] && string[l] != ' '; l++)
					user_data->user_name[l] = UPPER(string[l]);
				if (l == 32) {
#ifdef SUPERSERVER
					UTIL_error(GsecMsg76, NULL, NULL, NULL, NULL, NULL);
#else
					UTIL_print(GsecMsg76, NULL, NULL, NULL, NULL, NULL);
#endif
					/* invalid user name (maximum 31 bytes allowed) */
					return FALSE;
				}
				user_data->user_name[l] = '\0';
				user_data->user_name_entered = TRUE;
				break;
			case IN_SW_GSEC_PASSWORD:
				for (l = 0; l < 9 && string[l] && string[l] != ' '; l++)
					user_data->password[l] = string[l];
				if (l == 9) {
					UTIL_print(GsecMsg77, NULL, NULL, NULL, NULL, NULL);
					/* warning password maximum 8 significant bytes used */
				}
				user_data->password[l] = '\0';
				user_data->password_entered = TRUE;
				break;
			case IN_SW_GSEC_UID:
				user_data->uid = atoi(string);
				user_data->uid_entered = TRUE;
				break;
			case IN_SW_GSEC_GID:
				user_data->gid = atoi(string);
				user_data->gid_entered = TRUE;
				break;
			case IN_SW_GSEC_SYSU:
				strncpy(user_data->sys_user_name, string, 128);
				user_data->sys_user_entered = TRUE;
				break;
			case IN_SW_GSEC_GROUP:
				strncpy(user_data->group_name, string, 128);
				user_data->group_name_entered = TRUE;
				break;
			case IN_SW_GSEC_FNAME:
				strncpy(user_data->first_name, string, 17);
				user_data->first_name_entered = TRUE;
				break;
			case IN_SW_GSEC_MNAME:
				strncpy(user_data->middle_name, string, 17);
				user_data->middle_name_entered = TRUE;
				break;
			case IN_SW_GSEC_LNAME:
				strncpy(user_data->last_name, string, 17);
				user_data->last_name_entered = TRUE;
				break;
			case IN_SW_GSEC_DATABASE:
				strncpy(user_data->database_name, string, 512);
				user_data->database_entered = TRUE;
				break;
			case IN_SW_GSEC_DBA_USER_NAME:
				strncpy(user_data->dba_user_name, string, 133);
				user_data->dba_user_name_entered = TRUE;
				break;
			case IN_SW_GSEC_DBA_PASSWORD:
				strncpy(user_data->dba_password, string, 33);
				user_data->dba_password_entered = TRUE;
				break;
			case IN_SW_GSEC_SQL_ROLE_NAME:
				strncpy(user_data->sql_role_name, string, 33);
				user_data->sql_role_name_entered = TRUE;
				break;
			case IN_SW_GSEC_Z:
			case IN_SW_GSEC_0:
#ifdef SUPERSERVER
				UTIL_error(GsecMsg29, NULL, NULL, NULL, NULL, NULL);
#else
				UTIL_print(GsecMsg29, NULL, NULL, NULL, NULL, NULL);
#endif
				/* gsec - invalid parameter, no switch defined */
				return FALSE;
			}
			last_sw = IN_SW_GSEC_0;
		}
		else {
			/* iterate through the switch table, looking for matches */

			in_sw = IN_SW_GSEC_0;
			for (in_sw_tab = in_sw_table; q = in_sw_tab->in_sw_name;
				 in_sw_tab++) {
				p = string + 1;

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
					UTIL_error(GsecMsg30, NULL, NULL, NULL, NULL, NULL);
					/* gsec - operation already specified */
					return FALSE;
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
					*quitflag = TRUE;
					break;
				case IN_SW_GSEC_HELP:
					user_data->operation = HELP_OPER;
					break;
				}
				user_data->user_name[0] = '\0';
				tdsec->tsec_interactive = FALSE;
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
					user_data->password_specified = TRUE;
					user_data->password[0] = '\0';
					break;
				case IN_SW_GSEC_UID:
					if (user_data->uid_specified) {
						err_msg_no = GsecMsg32;
						break;
					}
					user_data->uid_specified = TRUE;
					user_data->uid = 0;
					break;
				case IN_SW_GSEC_GID:
					if (user_data->gid_specified) {
						err_msg_no = GsecMsg33;
						break;
					}
					user_data->gid_specified = TRUE;
					user_data->gid = 0;
					break;
				case IN_SW_GSEC_SYSU:
					if (user_data->sys_user_specified) {
						err_msg_no = GsecMsg34;
						break;
					}
					user_data->sys_user_specified = TRUE;
					user_data->sys_user_name[0] = '\0';
					break;
				case IN_SW_GSEC_GROUP:
					if (user_data->group_name_specified) {
						err_msg_no = GsecMsg35;
						break;
					}
					user_data->group_name_specified = TRUE;
					user_data->group_name[0] = '\0';
					break;
				case IN_SW_GSEC_FNAME:
					if (user_data->first_name_specified) {
						err_msg_no = GsecMsg36;
						break;
					}
					user_data->first_name_specified = TRUE;
					user_data->first_name[0] = '\0';
					break;
				case IN_SW_GSEC_MNAME:
					if (user_data->middle_name_specified) {
						err_msg_no = GsecMsg37;
						break;
					}
					user_data->middle_name_specified = TRUE;
					user_data->middle_name[0] = '\0';
					break;
				case IN_SW_GSEC_LNAME:
					if (user_data->last_name_specified) {
						err_msg_no = GsecMsg38;
						break;
					}
					user_data->last_name_specified = TRUE;
					user_data->last_name[0] = '\0';
					break;
				case IN_SW_GSEC_DATABASE:
					if (user_data->database_specified) {
						err_msg_no = GsecMsg78;
						break;
					}
					user_data->database_specified = TRUE;
					user_data->database_name[0] = '\0';
					break;
				case IN_SW_GSEC_DBA_USER_NAME:
					if (user_data->dba_user_name_specified) {
						err_msg_no = GsecMsg79;
						break;
					}
					user_data->dba_user_name_specified = TRUE;
					user_data->dba_user_name[0] = '\0';
					break;
				case IN_SW_GSEC_DBA_PASSWORD:
					if (user_data->dba_password_specified) {
						err_msg_no = GsecMsg80;
						break;
					}
					user_data->dba_password_specified = TRUE;
					user_data->dba_password[0] = '\0';
					break;
				case IN_SW_GSEC_SQL_ROLE_NAME:
					if (user_data->sql_role_name_specified) {
						err_msg_no = GsecMsg81;
						break;
					}
					user_data->sql_role_name_specified = TRUE;
					user_data->sql_role_name[0] = '\0';
					break;
				}
				if (err_msg_no) {
					UTIL_error(err_msg_no, NULL, NULL, NULL, NULL, NULL);
					return FALSE;
				}
				break;
			case IN_SW_GSEC_Z:
				if (!tdsec->tsec_sw_version) {
					SECURITY_msg_get(GsecMsg39, msg);
					util_output("%s %s\n", msg, GDS_VERSION);
				}
				tdsec->tsec_sw_version = TRUE;
				break;
			case IN_SW_GSEC_0:
#ifdef SUPERSERVER
				UTIL_error(GsecMsg40, NULL, NULL, NULL, NULL, NULL);
#else
				UTIL_print(GsecMsg40, NULL, NULL, NULL, NULL, NULL);
#endif
				/* gsec - invalid switch specified */
				return FALSE;
			case IN_SW_GSEC_AMBIG:
#ifdef SUPERSERVER
				UTIL_error(GsecMsg41, NULL, NULL, NULL, NULL, NULL);
#else
				UTIL_print(GsecMsg41, NULL, NULL, NULL, NULL, NULL);
#endif
				/* gsec - ambiguous switch specified */
				return FALSE;
			}
			last_sw = in_sw;
		}

		/* make sure that the current suite of switches and parameters
		   is valid, and if not, indicate why not */

		if (user_data->uid_entered || user_data->gid_entered ||
			user_data->sys_user_entered || user_data->group_name_entered ||
			user_data->password_entered || user_data->first_name_entered ||
			user_data->middle_name_entered || user_data->last_name_entered)
			switch (user_data->operation) {
			case 0:
				UTIL_error(GsecMsg42, NULL, NULL, NULL, NULL, NULL);
				/* gsec - no operation specified for parameters */
				return FALSE;
			case ADD_OPER:
			case MOD_OPER:
				/* any parameter is ok for these operation states */
				break;
			case DEL_OPER:
			case DIS_OPER:
			case QUIT_OPER:
			case HELP_OPER:
				UTIL_error(GsecMsg43, NULL, NULL, NULL, NULL, NULL);
				/* gsec - no parameters allowed for this operation */
				return FALSE;
			}

		if (*quitflag)
			break;
	}

	return TRUE;
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
	UTIL_print(GsecMsg45, NULL, NULL, NULL, NULL, NULL);
/* gsec utility - maintains user password database */

	util_output("\n%s", "   ");
	UTIL_print(GsecMsg46, NULL, NULL, NULL, NULL, NULL);
	util_output("%s", "     ");
	UTIL_print_partial(GsecMsg2, NULL, NULL, NULL, NULL, NULL);
	UTIL_print_partial(GsecMsg82, NULL, NULL, NULL, NULL, NULL);
	UTIL_print(GsecMsg47, NULL, NULL, NULL, NULL, NULL);
/* gsec [ <options> ... ] -<command> [ <parameter> ... ] */

	util_output("\n%s", "   ");
	UTIL_print(GsecMsg48, NULL, NULL, NULL, NULL, NULL);
/* interactive usage: */

	util_output("%s", "     ");
	UTIL_print_partial(GsecMsg2, NULL, NULL, NULL, NULL, NULL);
	UTIL_print(GsecMsg82, NULL, NULL, NULL, NULL, NULL);
/* gsec [ <options> ... ] */

	util_output("%s", "     ");
	UTIL_print_partial(GsecMsg1, NULL, NULL, NULL, NULL, NULL);
	util_output("\n%s", "     ");
	UTIL_print(GsecMsg47, NULL, NULL, NULL, NULL, NULL);
/* GSEC> <command> [ <parameter> ... ] */

	util_output("\n%s", "   ");
	UTIL_print(GsecMsg83, NULL, NULL, NULL, NULL, NULL);
/* available options: */

	util_output("%s", "     ");
	UTIL_print(GsecMsg84, NULL, NULL, NULL, NULL, NULL);
/* -user <database administrator name> */

	util_output("%s", "     ");
	UTIL_print(GsecMsg85, NULL, NULL, NULL, NULL, NULL);
/* -password <database administrator password> */

	util_output("%s", "     ");
	UTIL_print(GsecMsg86, NULL, NULL, NULL, NULL, NULL);
/* -role <database administrator SQL role name> */

	util_output("%s", "     ");
	UTIL_print(GsecMsg87, NULL, NULL, NULL, NULL, NULL);
/* -database <security database> */

	util_output("%s", "     ");
	UTIL_print(GsecMsg88, NULL, NULL, NULL, NULL, NULL);
/* -z */

	util_output("\n%s", "   ");
	UTIL_print(GsecMsg49, NULL, NULL, NULL, NULL, NULL);
/* available commands: */

	util_output("%s", "     ");
	UTIL_print(GsecMsg50, NULL, NULL, NULL, NULL, NULL);
/* adding a new user: */

	util_output("%s", "       ");
	UTIL_print(GsecMsg51, NULL, NULL, NULL, NULL, NULL);
/* add <name> [ <parameter> ... ] */

	util_output("%s", "     ");
	UTIL_print(GsecMsg52, NULL, NULL, NULL, NULL, NULL);
/* deleting a current user: */

	util_output("%s", "       ");
	UTIL_print(GsecMsg53, NULL, NULL, NULL, NULL, NULL);
/* delete <name> */

	util_output("%s", "     ");
	UTIL_print(GsecMsg54, NULL, NULL, NULL, NULL, NULL);
/* displaying all users: */

	util_output("%s", "       ");
	UTIL_print(GsecMsg55, NULL, NULL, NULL, NULL, NULL);
/* display */

	util_output("%s", "     ");
	UTIL_print(GsecMsg56, NULL, NULL, NULL, NULL, NULL);
/* displaying one user: */

	util_output("%s", "       ");
	UTIL_print(GsecMsg57, NULL, NULL, NULL, NULL, NULL);
/* display <name> */

	util_output("%s", "     ");
	UTIL_print(GsecMsg58, NULL, NULL, NULL, NULL, NULL);
/* modifying a user's parameters: */

	util_output("%s", "       ");
	UTIL_print(GsecMsg59, NULL, NULL, NULL, NULL, NULL);
/* modify <name> <parameter> [ <parameter> ... ] */

	util_output("%s", "     ");
	UTIL_print(GsecMsg60, NULL, NULL, NULL, NULL, NULL);
/* help: */

	util_output("%s", "       ");
	UTIL_print(GsecMsg61, NULL, NULL, NULL, NULL, NULL);
/* ? (interactive only) */

	util_output("%s", "       ");
	UTIL_print(GsecMsg62, NULL, NULL, NULL, NULL, NULL);
/* help */

	util_output("%s", "     ");
	UTIL_print(GsecMsg89, NULL, NULL, NULL, NULL, NULL);
/* displaying version number: */

	util_output("%s", "       ");
	UTIL_print(GsecMsg90, NULL, NULL, NULL, NULL, NULL);
/* z (interactive only) */

	util_output("%s", "     ");
	UTIL_print(GsecMsg63, NULL, NULL, NULL, NULL, NULL);
/* quit interactive session: */

	util_output("%s", "       ");
	UTIL_print(GsecMsg64, NULL, NULL, NULL, NULL, NULL);
/* quit (interactive only) */

	util_output("\n%s", "   ");
	UTIL_print(GsecMsg65, NULL, NULL, NULL, NULL, NULL);
/* available parameters: */

	util_output("%s", "     ");
	UTIL_print(GsecMsg66, NULL, NULL, NULL, NULL, NULL);
/* -pw <password> */

	util_output("%s", "     ");
	UTIL_print(GsecMsg67, NULL, NULL, NULL, NULL, NULL);
/* -uid <uid> */

	util_output("%s", "     ");
	UTIL_print(GsecMsg68, NULL, NULL, NULL, NULL, NULL);
/* -gid <gid> */

	util_output("%s", "     ");
	UTIL_print(GsecMsg71, NULL, NULL, NULL, NULL, NULL);
/* -fname <firstname> */

	util_output("%s", "     ");
	UTIL_print(GsecMsg72, NULL, NULL, NULL, NULL, NULL);
/* -mname <middlename> */

	util_output("%s", "     ");
	UTIL_print(GsecMsg73, NULL, NULL, NULL, NULL, NULL);
/* -lname <lastname> */
	util_output("\n", NULL);
}


static SSHORT parse_cmd_line( int argc, TEXT ** argv, TSEC tdsec)
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
	SSHORT ret;
	BOOLEAN quitflag = FALSE;
	USER_DATA user_data;

	user_data = tdsec->tsec_user_data;
	memset(user_data, 0, sizeof(USER_DATA));

/* Call a subroutine to process the input line. */

	ret = 0;
	if (!get_switches(argc, argv, gsec_in_sw_table, tdsec, &quitflag)) {
#ifdef SUPERSERVER
		UTIL_error(GsecMsg16, NULL, NULL, NULL, NULL, NULL);
#else
		UTIL_print(GsecMsg16, NULL, NULL, NULL, NULL, NULL);
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
				 !user_data->user_name_entered) {
			UTIL_error(GsecMsg18, NULL, NULL, NULL, NULL, NULL);
			/* gsec - no user name specified */
			ret = -1;
		}

	if (quitflag)
		ret = 1;

	if (tdsec->tsec_sw_version)
		ret = -1;

	return ret;
}

void UTIL_print_status( ISC_STATUS * status_vector)
{
/**************************************
 *
 *	U T I L _ p r i n t _ s t a t u s
 *
 **************************************
 *
 * Functional description
 *	Print error message. Use isc_interprete
 *	to allow redirecting output.
 *
 **************************************/
#ifdef SUPERSERVER
	TSEC tdsec;
	ISC_STATUS *status;
	int i = 0, j;
#endif
	ISC_STATUS *vector;
	SCHAR s[1024];

	if (status_vector) {
		vector = status_vector;
#ifdef SUPERSERVER
		tdsec = GET_THREAD_DATA;
		status = tdsec->tsec_service_blk->svc_status;
		if (status != status_vector) {
			while (*status && (++i < ISC_STATUS_LENGTH))
				status++;
			for (j = 0; status_vector[j] && (i < ISC_STATUS_LENGTH); j++, i++)
				*status++ = status_vector[j];
		}
#endif
		if (isc_interprete(s, &vector)) {
			TRANSLATE_CP(s);
			util_output("%s\n", s);
			while (isc_interprete(s, &vector)) {
				TRANSLATE_CP(s);
				util_output("%s\n", s);
			}
		}
	}
}

static void util_output( const SCHAR * format, ...)
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
	UCHAR buf[1000];
	int exit_code;
	TSEC tdsec;

	tdsec = GET_THREAD_DATA;

	if (format[0] == '\0') {
		exit_code =
			tdsec->tsec_output_proc(tdsec->tsec_output_data,
									(UCHAR * )(""));
	}
	else {
		VA_START(arglist, format);
		vsprintf((char *) buf, format, arglist);
		va_end(arglist);
		exit_code = tdsec->tsec_output_proc(tdsec->tsec_output_data, buf);
	}

	if (exit_code != 0)
		EXIT(exit_code);
}

void UTIL_error_redirect(
						 ISC_STATUS * status_vector,
						 USHORT errcode, TEXT * arg1, TEXT * arg2)
{
/**************************************
 *
 *	U T I L _ e r r o r _ r e d i r e c t
 *
 **************************************
 *
 * Functional description
 *	Issue error message. Output messages then abort.
 *
 **************************************/

	UTIL_print_status(status_vector);
	UTIL_error(errcode, arg1, arg2, NULL, NULL, NULL);
}

void UTIL_error(
				USHORT errcode,
				TEXT * arg1,
				TEXT * arg2, TEXT * arg3, TEXT * arg4, TEXT * arg5)
{
/**************************************
 *
 *	 U T I L _ e r r o r
 *
 **************************************
 *
 * Functional description
 *	Format and print an error message, then punt.
 *
 **************************************/
#ifdef SUPERSERVER
	TSEC tdsec;
	ISC_STATUS *status;

	tdsec = GET_THREAD_DATA;
	status = tdsec->tsec_service_blk->svc_status;

	CMD_UTIL_put_svc_status(status, GSEC_MSG_FAC, errcode,
							isc_arg_string, arg1,
							isc_arg_string, arg2,
							isc_arg_string, arg3,
							isc_arg_string, arg4, isc_arg_string, arg5);
	SVC_STARTED(tdsec->tsec_service_blk);
#else
	TSEC tdsec;
	tdsec = GET_THREAD_DATA;
#endif

	UTIL_print(errcode, arg1, arg2, arg3, arg4, arg5);
	EXIT(FINI_ERROR);
}

void UTIL_print(
				USHORT number,
				TEXT * arg1,
				TEXT * arg2, TEXT * arg3, TEXT * arg4, TEXT * arg5)
{
/**************************************
 *
 *	U T I L _ p r i n t
 *
 **************************************
 *
 * Functional description
 *	Retrieve a message from the error file, format it, and print it.
 *
 **************************************/
	TEXT buffer[256];

	gds__msg_format(0, GSEC_MSG_FAC, number, sizeof(buffer), buffer, arg1,
					arg2, arg3, arg4, arg5);
	TRANSLATE_CP(buffer);
	util_output("%s\n", buffer);
}

void UTIL_print_partial(
						USHORT number,
						TEXT * arg1,
						TEXT * arg2, TEXT * arg3, TEXT * arg4, TEXT * arg5)
{
/**************************************
 *
 *	U T I L _ p r i n t _ p a r t i a l
 *
 **************************************
 *
 * Functional description
 *	Retrieve a message from the error file, format it, and print it.
 *
 **************************************/
	TEXT buffer[256];

	gds__msg_format(0, GSEC_MSG_FAC, number, sizeof(buffer), buffer, arg1,
					arg2, arg3, arg4, arg5);
	TRANSLATE_CP(buffer);
	util_output("%s ", buffer);
}

