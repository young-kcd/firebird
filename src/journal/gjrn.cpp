/*
 *	PROGRAM:	JRD Journal Server
 *	MODULE:		gjrn.c
 *	DESCRIPTION:	
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
 * 2002.10.29 Sean Leyne - Removed obsolete "Netware" port
 *
 */

#include "firebird.h"
#include "../jrd/ib_stdio.h"
#include <setjmp.h>
#include <stdlib.h>
#include <string.h>
#include "../jrd/common.h"
#include <stdarg.h>
#include "../jrd/license.h"
#include "../jrd/gds.h"
#include "../journal/journal.h"
#include "../journal/conso_proto.h"
#include "../journal/gjrn_proto.h"
#include "../journal/misc_proto.h"
#include "../journal/rebui_proto.h"
#include "../journal/serve_proto.h"
#include "../jrd/gds_proto.h"

#if defined(WIN_NT)
#include <io.h>
#endif

USHORT sw_service_gjrn;
FILE *msg_file;

static void gjrn_msg_partial(USHORT, TEXT *, TEXT *, TEXT *, TEXT *, TEXT *);
static void gjrn_msg_put(USHORT, TEXT *, TEXT *, TEXT *, TEXT *, TEXT *);
static USHORT get_new_files(SCHAR **, SLONG *);
static int start_disable(int, SCHAR **);
static int start_dump(int, SCHAR **);
static int start_enable(int, SCHAR **);

static jmp_buf gjrn_env;

static UCHAR
	disable_dpb[] = { gds_dpb_version1, gds__dpb_disable_journal, 0 };

typedef struct func_tab {
	SCHAR *name;
	int (*func_routine) ();
} FUNC_TABLE;

static FUNC_TABLE option_table[] = {
#ifndef VMS
	"console", CONSOLE_start_console,
#endif
	"enable", start_enable,
	"disable", start_disable,
	"online_dump", start_dump,
	"restore", REBUILD_start_restore,
	"server", SERVER_start_server,
	0, 0
};


int CLIB_ROUTINE main( int argc,
					  char *argv[])
{
/**************************************
 *
 *	m a i n
 *
 **************************************
 *
 * Functional description
 *	Driver for the DBA tool utility.
 *	Parse switches and do work.
 *
 **************************************/
	SSHORT i, s_argc;
	SCHAR *p, **s_argv, string[512];
	USHORT found;
	USHORT sw_interactive = FALSE;
	SCHAR option_name[32];
	SLONG exit_code;
	SLONG redir_in, redir_out, redir_err;
	TEXT msg[128];
#ifdef VMS
	argc = VMS_parse(&argv, argc);
#endif

/* Perform some special handling when run as an Interbase service.  The
   first switch can be "-svc" (lower case!) or it can be "-svc_re" followed
   by 3 file descriptors to use in re-directing stdin, stdout, and stderr. */

	sw_service_gjrn = FALSE;

	if (argc > 1 && !strcmp(argv[1], "-svc")) {
		sw_service_gjrn = TRUE;
		argv++;
		argc--;
	}
	else if (argc > 4 && !strcmp(argv[1], "-svc_re")) {
		sw_service_gjrn = TRUE;
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

	try {

	s_argc = argc;
	s_argv = argv;
	msg_file = stdout;

	if (s_argc > 1) {
		s_argv++;
		s_argc--;
		MISC_down_case(s_argv[0], string);

		if (string[1] == 'z') {
			s_argc--;
			s_argv++;
			GJRN_printf(2, GDS_VERSION, NULL, NULL, NULL);
			if (s_argc == 0)
				exit(FINI_OK);
		}
	}
	else
		sw_interactive = TRUE;

	p = s_argv[0];

	if (sw_interactive) {
		p = option_name;
		GJRN_get_msg(OPTION_PROMPT, msg, NULL, NULL, NULL);	/* Msg 218 enter journal option: */
		if (!MISC_get_line(msg, p, sizeof(option_name)))
			MISC_print_journal_syntax();
	}
	else {
		p++;
		if (!p)
			MISC_print_journal_syntax();
	}

	found = FALSE;

	for (i = 0; option_table[i].name; i++) {
		if ((!strcmp(option_table[i].name, p)) ||
			((strlen(p) == 1) && (p[0] == option_table[i].name[0]))) {
			found = TRUE;
			break;
		}
	}

	if (!found)
		MISC_print_journal_syntax();

	if ((*option_table[i].func_routine) (s_argc, s_argv))
		exit(FINI_ERROR);
	else
		exit(FINI_OK);

	}	// try
	catch (const Firebird::status_exception& e) {
		exit(e.value());
	}
}


void GJRN_abort( int number)
{
/**************************************
 *
 *	G J R N _ a b o r t
 *
 **************************************
 *
 * Functional description
 *	Abandon a failed operation.
 *
 **************************************/

	if (number) {
		gjrn_msg_partial(0, 0, 0, 0, 0, 0);	/* msg 0: gbak:: */
		GJRN_printf(number, NULL, NULL, NULL, NULL);
	}

	gjrn_msg_partial(0, 0, 0, 0, 0, 0);	/* msg 0: gbak:: */
	GJRN_printf(1, NULL, NULL, NULL, NULL);	/* msg 1: exiting journal utility due to errors */

	Firebird::status_exception::raise(FINI_ERROR);
}


void GJRN_get_msg(
				  USHORT number,
				  TEXT * msg, TEXT * arg1, TEXT * arg2, TEXT * arg3)
{
/**************************************
 *
 *	 G J R N _ g e t _ m s g
 *
 **************************************
 *
 * Functional description
 *	Gets a string from the msg.fdb filling in the required params.
 *
 **************************************/

	gds__msg_format(0, GJRN_MSG_FAC, number, MSG_LENGTH, msg,
					arg1, arg2, arg3, 0, 0);
}


void GJRN_output( TEXT * format, ...)
{
/**************************************
 *
 *	G J R N _ o u t p u t
 *
 **************************************
 *
 * Functional description
 *	Display a formatted error message
 *
 **************************************/
	va_list ptr;
	TEXT buffer[MAXPATHLEN];

	VA_START(ptr, format);
	vsprintf(buffer, format, ptr);
	fprintf(stdout, "%s", buffer);
}


void GJRN_printf(
				 USHORT number,
				 TEXT * arg1, TEXT * arg2, TEXT * arg3, TEXT * arg4)
{
/**************************************
 *
 *	G J R N _ p r i n t f
 *
 **************************************
 *
 * Functional description
 *	Display a formatted error message
 *
 **************************************/

	gjrn_msg_put(number, arg1, arg2, arg3, arg4, 0);
}


void GJRN_print_syntax(void)
{
/**************************************
 *
 *	G J R N _ p r i n t _ s y n t a x
 *
 **************************************
 *
 * Functional description
 *	print possible options
 *
 **************************************/

	gjrn_msg_partial(0, 0, 0, 0, 0, 0);	/* msg 0: gbak:: */
	GJRN_printf(3, NULL, NULL, NULL, NULL);	/* msg 3: gjrn [-z] <utility> [-debug] [-verbose] [<options>] [<database name>] */
}


static void gjrn_msg_partial(
							 USHORT number,
							 TEXT * arg1,
							 TEXT * arg2,
							 TEXT * arg3, TEXT * arg4, TEXT * arg5)
{
/**************************************
 *
 *	g j r n _ m s g _ p a r t i a l
 *
 **************************************
 *
 * Functional description
 *	Retrieve a message from the error file, 
 *      format it, and print it
 *      without a newline.
 *
 **************************************/
	TEXT buffer[MAXPATHLEN];

	gds__msg_format(0, GJRN_MSG_FAC, number, sizeof(buffer), buffer,
					arg1, arg2, arg3, arg4, arg5);

	fprintf(msg_file, "%s", buffer);
}


static void gjrn_msg_put(
						 USHORT number,
						 TEXT * arg1,
						 TEXT * arg2, TEXT * arg3, TEXT * arg4, TEXT * arg5)
{
/**************************************
 *
 *	g j r n _ m s g _ p u t
 *
 **************************************
 *
 * Functional description
 *	Retrieve a message from the error file, format it, and print it.
 *
 **************************************/
	TEXT buffer[MAXPATHLEN];

	gds__msg_format(0, GJRN_MSG_FAC, number, sizeof(buffer), buffer,
					arg1, arg2, arg3, arg4, arg5);
	fprintf(msg_file, "%s\n", buffer);
}


static USHORT get_new_files( SCHAR ** old_files, SLONG * old_fs)
{
/**************************************
 *
 *	g e t _ n e w _ f i l e s
 *
 **************************************
 *
 * Functional description
 *	Get file names for online dump.  
 *	Returns number of files.
 *		file size.
 *
 **************************************/
	SSHORT num_files = 0;
	SCHAR *p, buff[MAXPATHLEN];
	SLONG fs = 0;
	SCHAR msg[MSG_LENGTH];

	GJRN_printf(4, NULL, NULL, NULL, NULL);	/* msg 4: reading online dump parameters */

	GJRN_get_msg(5, msg, 0, 0, 0);	/* msg 5: enter file size or <Ctrl-D> to end input */
	MISC_get_line(msg, buff, MAXPATHLEN);
	if (fs = atoi(buff))
		*old_fs = fs;

	while (TRUE) {
		GJRN_get_msg(6, msg, 0, 0, 0);	/* msg 6: enter file name or <Ctrl-D> to end input */
		buff[0] = 0;
		MISC_get_line(msg, buff, MAXPATHLEN);
		if (!strlen(buff))
			break;
		if (buff[0] == ' ')
			break;

		p = (SCHAR *) gds__alloc(MAXPATHLEN);
		strcpy(p, buff);
		old_files[num_files++] = p;
	}

	return num_files;
}


static int start_disable( int argc, SCHAR ** argv)
{
/**************************************
 *
 *	s t a r t _ d i s a b l e
 *
 **************************************
 *
 * Functional description
 *	Parse switches and do work.
 *
 **************************************/
	SLONG *handle;
	USHORT dpb_length, error;
	UCHAR *database, string[512], *dpb;
	ISC_STATUS status_vector[ISC_STATUS_LENGTH];
	SCHAR db_name[MAXPATHLEN];
	USHORT sw_v, sw_i;
	TEXT msg[128];

/* Start by parsing switches */

	sw_i = sw_v = FALSE;

	error = FALSE;
	database = NULL;
	argv++;

	while (--argc > 0) {
		if ((*argv)[0] != '-') {
			if (database) {
				GJRN_printf(12, database, NULL, NULL, NULL);	/* msg 12: database file name (%s) already specified */
				Firebird::status_exception::raise(FINI_ERROR);
			}
			database = *argv++;
			continue;
		}

		MISC_down_case(*argv++, string);
		switch (string[1]) {
		case 'v':
			sw_v = TRUE;
			break;

		case 'i':
			sw_i = TRUE;
			break;

		case 'd':
			break;

		default:
			MISC_print_journal_syntax();
			break;
		}
	}

	if ((sw_i) && (!database)) {
		GJRN_get_msg(219, msg, NULL, NULL, NULL);	/* enter database name: */
		if (MISC_get_line(msg, db_name, sizeof(db_name))) {
			database = db_name;
		}
	}

	if (!database) {
		GJRN_printf(13, NULL, NULL, NULL, NULL);	/* msg 13: please retry, giving a database name */
		MISC_print_journal_syntax();
	}

	dpb = disable_dpb;
	dpb_length = sizeof(disable_dpb);

	handle = NULL;
	gds__attach_database(status_vector,
						 0,
						 GDS_VAL(database),
						 GDS_REF(handle), dpb_length, GDS_VAL(dpb));

	if (status_vector[1]) {
		error = TRUE;
		gds__print_status(status_vector);
	}

	if (handle)
		gds__detach_database(status_vector, GDS_REF(handle));

	return error;
}


static int start_dump( int argc, SCHAR ** argv)
{
/**************************************
 *
 *	s t a r t _ d u m p
 *
 **************************************
 *
 * Functional description
 *	Parse switches and do work.
 *
 **************************************/
	SLONG *handle;
	USHORT dpb_length, error, i;
	UCHAR *database, string[512], *p, *q, *dpb;
	SCHAR db_name[MAXPATHLEN];
	ISC_STATUS status_vector[ISC_STATUS_LENGTH];
	USHORT old_num_files;
	SLONG old_file_size;
	UCHAR *old_files[20];
	ULONG start_page, start_seqno;
	USHORT start_file;
	SLONG fs;
	SSHORT dump_id;
	USHORT sw_d, sw_v, sw_i;
	TEXT msg[128];

/* Start by parsing switches */

	sw_i = sw_d = sw_v = FALSE;

	old_num_files = 0;
	old_file_size = 0;
	error = FALSE;
	database = NULL;
	argv++;

	while (--argc > 0) {
		if ((*argv)[0] != '-') {
			if (database) {
				GJRN_printf(12, database, NULL, NULL, NULL);	/* msg 12: database file name (%s) already specified */
				Firebird::status_exception::raise(FINI_ERROR);
			}
			database = *argv++;
			continue;
		}

		MISC_down_case(*argv++, string);
		switch (string[1]) {
		case 'd':
			sw_d = TRUE;
			break;

		case 'v':
			sw_v = TRUE;
			break;

		case 'i':
			sw_i = TRUE;
			break;

		case 'o':
			break;

		case 'f':
			if (--argc > 0)
				old_files[old_num_files++] = *argv++;
			else
				MISC_print_journal_syntax();
			break;

		case 's':
			if (--argc > 0) {
				old_file_size = atoi(*argv++);
				if (old_file_size <= 0) {
					GJRN_printf(16, NULL, NULL, NULL, NULL);	/* msg 16: online dump file size must be greater than zero */
					Firebird::status_exception::raise(FINI_ERROR);
				}
			}
			else
				MISC_print_journal_syntax();
			break;

		default:
			MISC_print_journal_syntax();
			break;
		}
	}

	if (sw_i) {
		if (!old_num_files)
			old_num_files = get_new_files(old_files, &fs);

		if (!old_file_size)
			old_file_size = fs;

		if (!database) {
			GJRN_get_msg(219, msg, NULL, NULL, NULL);	/* enter database name: */
			if (MISC_get_line(msg, db_name, sizeof(db_name))) {
				database = db_name;
			}
		}
	}

	if (!old_num_files)
		error = TRUE;

	if ((old_num_files > 1) && (!old_file_size))
		error = TRUE;

	if (error)
		MISC_print_journal_syntax();

	start_page = 0;
	start_seqno = 0;
	start_file = 0;
	dump_id = 0;
	fs = old_file_size;

	while (TRUE) {
		dpb = NULL;
		dpb_length = 0;

		p = dpb = string;
		*p++ = gds_dpb_version1;
		*p++ = gds__dpb_online_dump;
		*p++ = 1;
		*p++ = 1;

		if (dump_id) {
			*p++ = gds__dpb_old_dump_id;
			*p++ = 2;
			*p++ = dump_id % 256;
			*p++ = dump_id / 256;
		}

		if (old_file_size) {
			*p++ = gds__dpb_old_file_size;
			*p++ = 4;
			for (i = 0; i < 4; i++, old_file_size = old_file_size >> 8)
				*p++ = old_file_size;
		}

		if (start_page) {
			*p++ = gds__dpb_old_start_page;
			*p++ = 4;
			for (i = 0; i < 4; i++, start_page = start_page >> 8)
				*p++ = start_page;
		}

		if (start_seqno) {
			*p++ = gds__dpb_old_start_seqno;
			*p++ = 4;
			for (i = 0; i < 4; i++, start_seqno = start_seqno >> 8)
				*p++ = start_seqno;
		}

		*p++ = gds__dpb_old_num_files;
		*p++ = 1;
		*p++ = old_num_files;


		*p++ = gds__dpb_old_start_file;
		*p++ = 1;
		*p++ = start_file;

		for (i = 0; i < old_num_files; i++) {
			q = old_files[i];

			*p++ = gds__dpb_old_file;
			*p++ = strlen(q);
			for (; *q;)
				*p++ = *q++;
		}
		*p++ = 0;
		dpb_length = p - dpb;

		handle = NULL;
		gds__attach_database(status_vector,
							 0,
							 GDS_VAL(database),
							 GDS_REF(handle), dpb_length, GDS_VAL(dpb));

		dump_id = (USHORT) status_vector[3];
		start_page = status_vector[5];
		start_seqno = status_vector[7];
		start_file = status_vector[9];

		/* 
		 * Handle cases like online dump in progress etc. which are
		 * fatal.  Can continue only in case of no_space on disk error
		 */

		if ((error = status_vector[1])
			&& (status_vector[1] != gds__old_no_space)) {
			gds__print_status(status_vector);

			if (handle)
				gds__detach_database(status_vector, GDS_REF(handle));

			Firebird::status_exception::raise(FINI_ERROR);
		}

		if (handle)
			gds__detach_database(status_vector, GDS_REF(handle));

		/* Check if error && no space */

		if (sw_i && error) {
			if (old_num_files = get_new_files(old_files, &fs)) {
				old_file_size = fs;
				error = FALSE;

				/* read more file names and continue */

				continue;
			}
		}

		return error;
	}
}


static int start_enable( int argc, SCHAR ** argv)
{
/**************************************
 *
 *	s t a r t _ e n a b l e
 *
 **************************************
 *
 * Functional description
 *	Parse switches and do work.
 *
 **************************************/
	SLONG *handle;
	USHORT dpb_length, error;
	UCHAR *database, string[3 * MAXPATHLEN + 512], *p, *q, *dpb;
	UCHAR journal[JOURNAL_PATH_LENGTH + 1];
	UCHAR db_name[JOURNAL_PATH_LENGTH + 1];
	UCHAR backup[MAXPATHLEN];
	ISC_STATUS status_vector[ISC_STATUS_LENGTH];
	USHORT sw_d, sw_v, sw_i;
	USHORT a_flag, j_flag;
	TEXT msg[128];

/* Start by parsing switches */

	sw_i = sw_d = sw_v = a_flag = j_flag = FALSE;

	error = FALSE;
	database = NULL;
	strcpy(journal, ".");
	argv++;

	while (--argc > 0) {
		if ((*argv)[0] != '-') {
			if (database) {
				GJRN_printf(12, database, NULL, NULL, NULL);	/* msg 12: database file name (%s) already specified */
				Firebird::status_exception::raise(FINI_ERROR);
			}
			database = *argv++;
			continue;
		}

		MISC_down_case(*argv++, string);
		switch (string[1]) {
		case 'a':
			if (--argc > 0) {
				a_flag = TRUE;
				strcpy(backup, *argv++);
			}
			else
				MISC_print_journal_syntax();
			break;

		case 'v':
			sw_v = TRUE;
			break;

		case 'i':
			sw_i = TRUE;
			break;

		case 'd':
			sw_d = TRUE;
			break;

		case 'e':
			break;

		case 'j':
			if (--argc > 0) {
				strcpy(journal, *argv++);
				j_flag = TRUE;
			}
			else
				MISC_print_journal_syntax();
			break;

		default:
			MISC_print_journal_syntax();
			break;
		}
	}

	if (sw_i) {
		if (!database) {
			GJRN_get_msg(DB_PROMPT, msg, NULL, NULL, NULL);	/* Msg 219 enter database name: */
			if (MISC_get_line(msg, db_name, sizeof(db_name))) {
				database = db_name;
			}
		}
		if (!a_flag) {
			GJRN_get_msg(BAK_DIR_PROMPT, msg, NULL, NULL, NULL);	/* Msg 220 enter backup directory name: */
			if (MISC_get_line(msg, backup, sizeof(backup))) {
				a_flag = TRUE;
			}
		}
		if (!j_flag) {
			GJRN_get_msg(JRN_DIR_PROMPT, msg, NULL, NULL, NULL);	/* Msg 91 enter journal directory name: */
			if (MISC_get_line(msg, journal, sizeof(journal))) {
				j_flag = TRUE;
			}
		}
	}

	if (!database) {
		GJRN_printf(13, NULL, NULL, NULL, NULL);	/* msg 13: please retry, giving a database name */
		MISC_print_journal_syntax();
	}

	dpb = NULL;
	dpb_length = 0;

	p = dpb = string;
	*p++ = gds_dpb_version1;
	*p++ = gds__dpb_enable_journal;
	*p++ = strlen(journal);
	for (q = journal; *q;)
		*p++ = *q++;

	if (a_flag) {
		*p++ = gds__dpb_wal_backup_dir;
		*p++ = strlen(backup);
		for (q = backup; *q;)
			*p++ = *q++;
	}
	dpb_length = p - dpb;

	handle = NULL;
	gds__attach_database(status_vector,
						 0,
						 GDS_VAL(database),
						 GDS_REF(handle), dpb_length, GDS_VAL(dpb));

	if (status_vector[1]) {
		error = TRUE;
		gds__print_status(status_vector);
	}

	if (handle)
		gds__detach_database(status_vector, GDS_REF(handle));

	return error;
}
