/*
 *	PROGRAM:	JRD Remote Server
 *	MODULE:		inet_server.c
 *	DESCRIPTION:	Internet remote server.
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
 * Added TCP_NO_DELAY option for superserver on Linux
 * FSG 16.03.2001 
 *
 * 2002.10.28 Sean Leyne - Code cleanup, removed obsolete "MPEXL" port
 * 2002.10.28 Sean Leyne - Code cleanup, removed obsolete "DecOSF" port
 * 2002.10.28 Sean Leyne - Code cleanup, removed obsolete "SGI" port
 *
 * 2002.10.29 Sean Leyne - Removed obsolete "Netware" port
 *
 * 2002.10.30 Sean Leyne - Removed support for obsolete "PC_PLATFORM" define
 *
 */
/*
$Id: inet_server.cpp,v 1.26.2.3 2005-03-23 12:59:25 alexpeshkoff Exp $
*/
#include "firebird.h"
#include "../jrd/ib_stdio.h"
#include <stdlib.h>
#include "../jrd/common.h"
#include "../jrd/isc_proto.h"
#include "../jrd/divorce.h"
#include "../common/config/config.h"
#if !(defined VMS)
#include <sys/param.h>
#endif

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#if TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# if HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#if HAVE_SYS_WAIT_H
# include <sys/wait.h>
#endif
#ifndef WEXITSTATUS
# define WEXITSTATUS(stat_val) ((unsigned)(stat_val) >> 8)
#endif
#ifndef WIFEXITED
# define WIFEXITED(stat_val) (((stat_val) & 255) == 0)
#endif

#ifdef HAVE_STRING_H
#include <string.h>
#endif


#ifdef WINDOWS_ROUTER
#define MAX_ARGS	6
#endif /* WINDOWS_ROUTER */

#ifdef VMS
#include <descrip.h>
#endif

#ifdef SUPERSERVER
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <errno.h>
#include "../jrd/gds.h"
#include "../jrd/jrd_pwd.h"
#endif

#include "../remote/remote.h"
#include "../jrd/license.h"
#include "../jrd/thd.h"
#include "../jrd/file_params.h"
#include "../remote/inet_proto.h"
#include "../remote/serve_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/sch_proto.h"

#ifdef UNIX
#ifdef NETBSD
#include <signal.h>
#else
#include <sys/signal.h>
#endif
#endif

#ifndef hpux
#define sigvector	sigvec
#endif

#ifndef NBBY
#define	NBBY		8
#endif

#ifndef SV_INTERRUPT
#define SV_INTERRUPT	0
#endif

#ifndef NFDBITS
#define NFDBITS		(sizeof(SLONG) * NBBY)

#if !(defined DARWIN)
#define	FD_SET(n, p)	((p)->fds_bits[(n)/NFDBITS] |= (1 << ((n) % NFDBITS)))
#define	FD_CLR(n, p)	((p)->fds_bits[(n)/NFDBITS] &= ~(1 << ((n) % NFDBITS)))
#define	FD_ISSET(n, p)	((p)->fds_bits[(n)/NFDBITS] & (1 << ((n) % NFDBITS)))
#define FD_ZERO(p)	memset((SCHAR *)(p), 0, sizeof(*(p)))
#endif
#endif

#ifdef SUPERSERVER
#ifndef WIN_NT
#define TEMP_DIR "/tmp"
#define CHANGE_DIR chdir
#endif
#define INTERBASE_USER_NAME     "interbase"
#define INTERBASE_USER_SHORT    "interbas"
#define FIREBIRD_USER_NAME	"firebird"
#endif


extern "C" {


static int assign(SCHAR *);
static void name_process(UCHAR *);
static void signal_handler(void);
#ifdef SUPERSERVER
static void signal_sigpipe_handler(void);
#endif
static void set_signal(int, FPTR_VOID);

#ifdef WINDOWS_ROUTER
static int atov(UCHAR *, UCHAR **, SSHORT);
#endif /* WINDOWS_ROUTER */

static TEXT protocol[128];
static int INET_SERVER_start = 0;
static USHORT INET_SERVER_flag = 0;

#ifdef WINDOWS_ROUTER
int PASCAL WinMain(
				   HINSTANCE hInstance,
				   HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
#else /* WINDOWS_ROUTER */
int CLIB_ROUTINE server_main( int argc, char **argv)
#endif							/* WINDOWS_ROUTER */
{
/**************************************
 *
 *	m a i n
 *
 **************************************
 *
 * Functional description
 *	Run the server with apollo mailboxes.
 *
 **************************************/
	int n, clients;
	PORT port;
	int child, debug, channel, standalone, multi_threaded, multi_client;
	TEXT *p, **end, c;
	int done = FALSE;
#if !(defined VMS)
	fd_set mask;
#endif

#ifdef WINDOWS_ROUTER
/*
 *	Construct an argc, argv so we can use the old parse code.
 */
	int argc;
	char **argv, *argv2[MAX_ARGS];

	argv = argv2;
	argv[0] = "IB_server";
	argc = 1 + atov(lpszCmdLine, argv + 1, MAX_ARGS - 1);

#endif /* WINDOWS_ROUTER */


#ifdef VMS
	argc = VMS_parse(&argv, argc);
#endif

	end = argc + argv;
	argv++;
	debug = standalone = INET_SERVER_flag = FALSE;
	channel = 0;
	protocol[0] = 0;
	multi_client = multi_threaded = FALSE;

#ifdef SUPERSERVER
	INET_SERVER_flag |= SRVR_multi_client;
	multi_client = multi_threaded = standalone = TRUE;
#endif

	clients = 0;

	while (argv < end) {
		p = *argv++;
		if (*p++ == '-')
			while (c = *p++) {
				switch (UPPER(c)) {
				case 'D':
					INET_SERVER_flag |= SRVR_debug;
					debug = standalone = TRUE;
#ifdef NEVERDEF
#ifdef SUPERSERVER
					free_map_debug = 1;
#endif
#endif
					break;
#ifndef SUPERSERVER

				case 'M':
					INET_SERVER_flag |= SRVR_multi_client;
					if (argv < end)
						if (clients = atoi(*argv))
							argv++;
					multi_client = standalone = TRUE;
					break;

				case 'S':
					standalone = TRUE;
					break;


				case 'I':
					standalone = FALSE;
					break;

				case 'T':
					multi_threaded = TRUE;
					break;

				case 'U':
					multi_threaded = FALSE;
					break;
#endif /* SUPERSERVER */

				case 'E':
					if (ISC_get_prefix(p) == -1)
						ib_printf("Invalid argument Ignored\n");
					else
						argv++;	/* donot skip next argument if this one 
								   is invalid */
					done = TRUE;
					break;

				case 'P':
					protocol[0] = '/';
					protocol[1] = 0;
					strncat(protocol, *argv++, 
						sizeof(protocol) - strlen(protocol) - 1);
					break;

                case 'H':
				case '?':
					ib_printf("Firebird TCP/IP server options are:\n");
					ib_printf("  -d           : debug on\n");
                   
#ifdef SUPERSERVER
                    // These options only applicable to super server
					ib_printf("  -m           : multiclient - on\n");
					ib_printf("  -s           : standalone - true\n");
					ib_printf("  -i           : standalone - false\n");

					ib_printf("  -t           : multithread - true  (non pc only)\n");
					ib_printf("  -u           : multithread - false (pc only)\n");
					ib_printf("  -t           : multithread (non pc only\n");
#endif

					ib_printf("  -p<protocol> : specify protocol\n");
					ib_printf("  -h|? : print this help\n");
                    ib_printf("\n");
                    ib_printf("  (The following -e options used to be -h options)\n");
					ib_printf("  -e<firebird_root_dir>   : set firebird_root path\n");            
					ib_printf("  -el<firebird_lock_dir>   : set runtime firebird_lock dir\n");            
					ib_printf("  -em<firebird_msg_dir>   : set firebird_msg dir path\n");            
					ib_printf("  -z   : print version\n");            

					exit(FINI_OK);
				case 'Z':
					ib_printf("Firebird TCP/IP server version %s\n",
							  GDS_VERSION);
					exit(FINI_OK);
				}
				if (done)
					break;
			}
	}
#if (defined SUPERSERVER && defined UNIX )
/*	set_signal(SIGPIPE, signal_sigpipe_handler); */
    set_signal (SIGPIPE, signal_handler);
	set_signal(SIGUSR1, signal_handler);
	set_signal(SIGUSR2, signal_handler);
#endif

/* Fork off a server, wait for it to die, then fork off another,
   but give up after 100 tries */

#if !(defined SUPERSERVER || defined VMS)
	if (multi_client && !debug) {
		set_signal(SIGUSR1, signal_handler);
		for (n = 0; n < 100; n++) {
			INET_SERVER_start = 0;
			if (!(child = fork()))
				break;
			while (wait(0) != child)
				if (INET_SERVER_start) {
					n = 0;		/* reset error counter on "real" signal */
					break;
				}
			gds__log("INET_SERVER/main: gds_inet_server restarted");
		}
		set_signal(SIGUSR1, (void(*)()) SIG_DFL);
	}
#endif

	if (standalone) {
		if (multi_client) {
#ifdef SUPERSERVER

            // Remove restriction on username, for DEV builds
            // restrict only for production builds.  MOD 21-July-2002
#ifndef DEV_BUILD
			TEXT user_name[256];	/* holds the user name */
			/* check user id */
			ISC_get_user(user_name, NULL, NULL, NULL, NULL, NULL, NULL);

			if (strcmp(user_name, "root") &&
				strcmp(user_name, FIREBIRD_USER_NAME) &&
				strcmp(user_name, INTERBASE_USER_NAME) &&
				strcmp(user_name, INTERBASE_USER_SHORT)) {
				/* invalid user -- bail out */
				ib_fprintf(ib_stderr,
						   "%s: Invalid user (must be %s, %s, %s or root).\n",
						   "fbserver", FIREBIRD_USER_NAME,
						   INTERBASE_USER_NAME, INTERBASE_USER_SHORT);
				exit(STARTUP_ERROR);
			}
#endif
#else
			if (setreuid(0, 0) < 0)
				ib_printf("Inet_server: couldn't set uid to superuser.\n");
#endif
			INET_set_clients(clients);
		}

		if (!debug) {
			FD_ZERO(&mask);
			FD_SET(2, &mask);
			divorce_terminal((int) &mask);
		}
		{
			ISC_STATUS_ARRAY status_vector;
			THREAD_ENTER;
			port = INET_connect(protocol, 0, status_vector, INET_SERVER_flag,
								0, 0);
			THREAD_EXIT;
			if (!port) {
				gds__print_status(status_vector);
				exit(STARTUP_ERROR);
			}
		}
	}
	else {
#ifdef VMS
		channel = assign("SYS$INPUT");
#endif
		THREAD_ENTER;
		port = INET_server(channel);
		THREAD_EXIT;
		if (!port) {
			ib_fprintf(ib_stderr, "fbserver: Unable to start INET_server\n");
			exit(STARTUP_ERROR);
		}
	}

#ifdef SUPERSERVER

/* before starting the superserver stuff change directory to tmp */
	if (CHANGE_DIR(TEMP_DIR)) {
		/* error on changing the directory */
		gds__log("Could not change directory to %s due to errno %d",
				TEMP_DIR, errno);
	}

/* Server tries to attash to security.fdb to make sure everything is OK
   This code fixes bug# 8429 + all other bug of that kind - from 
   now on the server exits if it cannot attach to the database
   (wrong or no license, not enough memory, etc.
*/
	{
		TEXT path[MAXPATHLEN];
		ISC_STATUS_ARRAY status;
		isc_db_handle db_handle = 0L;

		gds__prefix(path, USER_INFO_NAME);
		isc_attach_database(status, strlen(path), path, &db_handle, 0, NULL);
		if (status[0] == 1 && status[1] > 0) {
			gds__log_status(USER_INFO_NAME, status);
			isc_print_status(status);
			exit(STARTUP_ERROR);
		}
		isc_detach_database(status, &db_handle);
		if (status[0] == 1 && status[1] > 0) {
			gds__log_status(USER_INFO_NAME, status);
			isc_print_status(status);
			exit(STARTUP_ERROR);
		}
	}

#endif

	if (multi_threaded)
		SRVR_multi_thread(port, INET_SERVER_flag);
	else
#ifdef WINDOWS_ROUTER
		SRVR_WinMain(port, INET_SERVER_flag, hInstance, hPrevInstance,
					 nCmdShow);
#else
		SRVR_main(port, INET_SERVER_flag);
#endif

#ifdef DEBUG_GDS_ALLOC
/* In Debug mode - this will report all server-side memory leaks
 * due to remote access
 */
	//gds_alloc_report(0, __FILE__, __LINE__);
	char name[MAXPATHLEN];
	gds__prefix(name, "memdebug.log");
	FILE* file = fopen(name, "w+b");
	if (file) {
	  fprintf(file,"Global memory pool allocated objects\n");
	  getDefaultMemoryPool()->print_contents(file);
	  fclose(file);
	}
#endif

	exit(FINI_OK);
}


#ifdef WINDOWS_ROUTER
static int atov( UCHAR * str, UCHAR ** vec, SSHORT len)
{
/**************************************
 *
 *	a t o v
 *
 **************************************
 *
 * Functional description
 *	Take a string and convert it to a vector.
 *	White space delineates, but things in quotes are
 *	kept together.
 *
 **************************************/
	int i = 0, qt = 0, qq;
	char *p1, *p2;

	vec[0] = str;
	for (p1 = p2 = str; i < len; i++) {
		while (*p1 == ' ' || *p1 == '\t')
			p1++;
		while (qt || (*p1 != ' ' && *p1 != '\t')) {
			qq = qt;
			if (*p1 == '\'')
				if (!qt)
					qt = -1;
				else if (qt == -1)
					qt = 0;
			if (*p1 == '"')
				if (!qt)
					qt = 1;
				else if (qt == 1)
					qt = 0;
			if (*p1 == '\0' || *p1 == '\n') {
				*p2++ = '\0';
				vec[++i] = 0;
				return i;
			}
			if (qq == qt)
				*p2++ = *p1;
			p1++;
		}
		p1++;
		*p2++ = '\0';
		vec[i + 1] = p2;
	}
	*p2 = '\0';
	vec[i] = 0;
	return i - 1;
}
#endif /* WINDOWS_ROUTER */


#ifdef VMS
static int assign( SCHAR * string)
{
/**************************************
 *
 *	a s s i g n
 *
 **************************************
 *
 * Functional description
 *	Assign a channel for communications.
 *
 **************************************/
	SSHORT channel;
	int status;
	struct dsc$descriptor_s desc;

	desc.dsc$b_dtype = DSC$K_DTYPE_T;
	desc.dsc$b_class = DSC$K_CLASS_S;
	desc.dsc$w_length = strlen(string);
	desc.dsc$a_pointer = string;

	status = sys$assign(&desc, &channel, NULL, NULL);

	return (status & 1) ? channel : 0;
}
#endif





#if !(defined VMS)
static void set_signal( int signal_number, void (*handler) (void))
{
/**************************************
 *
 *	s e t _ s i g n a l
 *
 **************************************
 *
 * Functional description
 *	Establish signal handler.
 *
 **************************************/
#ifdef SYSV_SIGNALS
	sigset(signal_number, handler);
#else

#ifndef HAVE_SIGACTION
	struct sigvec vec;
	struct sigvec old_vec;

	vec.sv_handler = handler;
	vec.sv_mask = 0;
	vec.sv_flags = SV_INTERRUPT;
	sigvector(signal_number, &vec, &old_vec);
#else
	struct sigaction vec, old_vec;

	vec.sa_handler = (SIG_FPTR) handler;
	memset(&vec.sa_mask, 0, sizeof(vec.sa_mask));
	vec.sa_flags = 0;
	sigaction(signal_number, &vec, &old_vec);
#endif

#endif
}
#endif


static void signal_handler(void)
{
/**************************************
 *
 *	s i g n a l _ h a n d l e r
 *
 **************************************
 *
 * Functional description
 *	Dummy signal handler.
 *
 **************************************/

	++INET_SERVER_start;
}

#if (defined SUPERSERVER && defined UNIX )
static void signal_sigpipe_handler(void)
{
/****************************************************
 *
 *	s i g n a l _ s i g p i p e _ h a n d l e r
 *
 ****************************************************
 *
 * Functional description
 *	Dummy signal handler.
 *
 **************************************/

	++INET_SERVER_start;
	gds__log
		("Super Server/main: Bad client socket, send() resulted in SIGPIPE, caught by server\n                   client exited improperly or crashed ????");
}
#endif


} // extern "C"
