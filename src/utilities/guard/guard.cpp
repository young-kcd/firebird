/*
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
 * $Id: guard.cpp,v 1.1 2003-07-15 02:39:38 brodsom Exp $
 */
 /* contains the main() and not shared routines for ibguard */

#define FOREVER		1
#define ONETIME		2
#define IGNORE          3
#define NORMAL_EXIT	0

#define SUPER_SERVER_BINARY	"bin/fbserver"

#define INTERBASE_USER		"interbase"
#define FIREBIRD_USER		"firebird"
#define INTERBASE_USER_SHORT	"interbas"

#include "firebird.h"
#include "../jrd/ib_stdio.h"

#ifdef HAVE_STRING_H
#include <string.h>
#endif

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif

#include "../jrd/common.h"
#include "../jrd/isc_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/file_params.h"
#include "../utilities/guard/util_proto.h"


int CLIB_ROUTINE main( int argc, char **argv)
{
/**************************************
 *
 *      m a i n
 *
 **************************************
 *
 * Functional description
 *      The main for ibguard. This process is used to start
 *      the super server (fbserver) and keep it running
 *	after an abnormal termination.
 *
 *      process takes 1 argument:  -f (default) or -o
 *
 **************************************/
	TEXT **end, *p, c;
	USHORT option = FOREVER;	/* holds FOREVER or ONETIME */
	TEXT user_name[256];		/* holds the user name */
	USHORT done = TRUE, fd_guard;
	pid_t child_pid;
	TEXT *prog_name = argv[0];
	TEXT *server_args[2];
	TEXT process_name[1024];

	ULONG old_mask;
	ULONG new_mask = 0000;

	end = argc + argv;
	argv++;
	while (argv < end) {
		p = *argv++;
		if (*p++ == '-')
			switch (UPPER(*p)) {
			case 'F':
				option = FOREVER;
				break;
			case 'O':
				option = ONETIME;
				break;
			case 'S':
				option = IGNORE;
				break;
			default:
				ib_fprintf(ib_stderr,
						   "Usage: %s [-signore | -onetime | -forever (default)]\n",
						   prog_name);
				exit(-1);
				break;
			}

	}							/* while */

/* check user id */
	ISC_get_user(user_name, NULL, NULL, NULL, NULL, NULL, NULL);

	if (strcmp(user_name, INTERBASE_USER) && strcmp(user_name, "root")
		&& strcmp(user_name, FIREBIRD_USER)
		&& strcmp(user_name, INTERBASE_USER_SHORT)) {
		/* invalid user bail out */
		ib_fprintf(ib_stderr,
				   "%s: Invalid user (must be %s, %s, %s or root).\n",
				   prog_name, FIREBIRD_USER, INTERBASE_USER,
				   INTERBASE_USER_SHORT);
		exit(-2);
	}

/* get and set the umask for the current process */
	old_mask = umask(new_mask);

/* exclusive lock the file */
	if ((fd_guard = UTIL_ex_lock(GUARD_FILE)) < 0) {
		/* could not get exclusive lock -- some other guardian is running */
		if (fd_guard == -2)
			ib_fprintf(ib_stderr, "%s: Program is already running.\n",
					   prog_name);
		exit(-3);
	}

/* the umask back to orignal donot want to carry this to child process */
	umask(old_mask);

/* move the server name into the argument to be passed */
	server_args[0] = process_name;
	server_args[1] = NULL;

	do {
		int ret_code;

		gds__log("%s: guardian starting %s\n",
				 prog_name, SUPER_SERVER_BINARY);
		child_pid = UTIL_start_process(SUPER_SERVER_BINARY, server_args);
		if (child_pid == -1) {
			/* could not fork the server */
			gds__log("%s: guardian could not start %s\n", prog_name,
					 server_args[1] ? server_args[1] : SUPER_SERVER_BINARY);
			ib_fprintf(ib_stderr, "%s: Could not start %s\n", prog_name,
					   server_args[1] ? server_args[1] : SUPER_SERVER_BINARY);
			UTIL_ex_unlock(fd_guard);
			exit(-4);
		}

		/* wait for child to die, and evaluate exit status */
		ret_code = UTIL_wait_for_child(child_pid);
		if (ret_code != NORMAL_EXIT) {
			/* check for startup error */
			if (ret_code == STARTUP_ERROR) {
				gds__log("%s: %s terminated due to startup error (%d)\n",
						 prog_name, server_args[1] ? server_args[1] :
						 SUPER_SERVER_BINARY, ret_code);
				if (option == IGNORE) {
					gds__log
						("%s: %s terminated due to startup error (%d)\n Trying again\n",
						 prog_name,
						 server_args[1] ? server_args[1] :
						 SUPER_SERVER_BINARY, ret_code);

					done = FALSE;	/* Try it again, Sam (even if it is a startup error) FSG 8.11.2000 */
				}
				else {
					gds__log("%s: %s terminated due to startup error (%d)\n",
							 prog_name, server_args[1] ? server_args[1] :
							 SUPER_SERVER_BINARY, ret_code);

					done = TRUE;	/* do not restart we have a startup problem */
				}
			}
			else {
				gds__log("%s: %s terminated abnormally (%d)\n", prog_name,
						 server_args[1] ? server_args[1] :
						 SUPER_SERVER_BINARY, ret_code);
				if (option == FOREVER || option == IGNORE)
					done = FALSE;
			}
		}
		else {
			/* Normal shutdown - eg: via ibmgr - don't restart the server */
			gds__log("%s: %s normal shutdown.\n", prog_name,
					 server_args[1] ? server_args[1] : SUPER_SERVER_BINARY);
			done = TRUE;
		}
	} while (!done);

	UTIL_ex_unlock(fd_guard);
	exit(0);
}								/* main */
