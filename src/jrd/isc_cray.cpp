#error This file is only renamed from .c to .cpp and indented.
#error It still has old K&R style function arguments.
#error The Cray port is obsolete!
/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		isc_cray.c
 *	DESCRIPTION:	General purpose but non-user routines.
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
 * 2002.02.15 Sean Leyne - Code Cleanup, removed obsolete "XENIX" port
 * 2002.10.27 Sean Leyne - Code Cleanup, removed obsolete "Ultrix" port
 *
 * 2002.10.28 Sean Leyne - Completed removal of obsolete "DGUX" port
 *
 */

#include "firebird.h"
#include "../jrd/common.h"
#include "gen/codes.h"

#include "../jrd/ib_stdio.h"
#include <pwd.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/signal.h>
#include <errno.h>

#ifndef O_RDWR
#include <fcntl.h>
#endif

#define STACK_SIZE	32000

typedef struct mnt {
	TEXT *mnt_node;
	TEXT *mnt_mount;
	TEXT *mnt_path;
} MNT;

typedef struct sig {
	struct sig *sig_next;
	int sig_signal;
	void (*sig_routine) ();
	SLONG sig_arg;
	SLONG sig_count;
	USHORT sig_flags;
} *SIG;

#define SIG_client	1			/* Not our routine */
#define SIGVEC		struct sigvec
#define INET_FLAG	':'
#define MTAB		"/etc/mtab"
#define sigvector	sigvec

TEXT *ISC_hostname();
extern UCHAR *gds__alloc();

static cleanup(), overflow_handler();
static void signal_handler();
static SIG que_signal();

static TEXT user_name[256];
static SIG signals;
static USHORT blocked;
static USHORT inhibit_count, ast_count;
static SLONG overflow_count;
static SLONG pending_signals;
static int process_id;
static SIGVEC client_sigfpe;


/* Unix specific stuff */

#define MAXPATHLEN	1024
#define MAXHOSTLEN	64

#define FTOK_KEY	15
#define PRIV		0666
#define LOCAL_SEMAPHORES 4

#ifndef GDS_RELAY
#define GDS_RELAY	"/bin/gds_relay"
#endif

typedef struct {
	SSHORT event_semid;
	SSHORT event_semnum;
	SLONG event_count;
} *EVENT;

extern struct passwd *getpwnam(), *getpwuid();
extern SLONG ftok();

static relay_pipe, local_semid, local_semaphores;



ISC_analyze_filename(file_name, node_name)
	 TEXT *file_name, *node_name;
{
/**************************************
 *
 *	I S C _ a n a l y z e _ f i l e n a m e		( G E N E R I C )
 *
 **************************************
 *
 * Functional description
 *	Analyze a filename for a TCP node name on the front.  If
 *	one is found, extract the node name, compute the residual
 *	file name, and return TRUE.  Otherwise return FALSE.
 *
 **************************************/
	TEXT *p;

/* Scan file name looking for separator character */

	p = file_name;

	while (*p && *p != INET_FLAG)
		*node_name++ = *p++;

	if (!*p)
		return FALSE;

/* for OS/2, introduce a restriction against one-character
   machine names as a kludge to prevent the situation of
   trying to attach to C: as a remote machine -- there has
   got to be a better way to resolve this */

#ifdef MSDOS
	if (p - file_name == 1)
		return FALSE;
#endif

	*node_name = 0;

	while (*file_name++ = *++p);

	return TRUE;
}




ISC_analyze_nfs(expanded_filename, node_name)
	 TEXT *expanded_filename, *node_name;
{
/**************************************
 *
 *	I S C _ a n a l y z e _ n f s
 *
 **************************************
 *
 * Functional description
 *	Check a file name for an NFS mount point.  If so, decompose
 *	into node name and remote file name.
 *
 **************************************/
	MNT mount;
	TEXT mnt_buffer[1000], remote_filename[MAXPATHLEN], *p, *q, *temp,
		max_node[MAXHOSTLEN], max_path[MAXPATHLEN], expand_mount[MAXPATHLEN];
	USHORT flag;
	IB_FILE *mtab;
	int context, len;

	len = 0;
	*max_path = 0;
	flag = FALSE;

/* Search mount points */

	temp = NULL;
	context = 0;

#if (defined AIX || defined AIX_PPC)
	while (get_mounts(&mount, mnt_buffer, &temp, &context))
#else
	if (!(mtab = ib_fopen(MTAB, "r")))
		return flag;

	while (get_mounts(&mount, mnt_buffer, mtab))
#endif
	{
		/* first, expand any symbolic links in the mount point */

		ISC_expand_filename(mount.mnt_mount, 0, expand_mount);
		mount.mnt_mount = expand_mount;

		/* see how much of the mount point matches the expanded_filename */

		p = expanded_filename;
		for (q = mount.mnt_mount; *q && *q == *p++; q++);

		/* if the whole mount point is not contained in the expanded_filename
		   OR the mount point is not a valid pathname in the expanded_filename,
		   skip it */

		if (*q || *p != '/')
			if (!strcmp(mount.mnt_mount, "/") && *mount.mnt_path) {
				/* root mount point = diskless client case */

				strcat(mount.mnt_path, "/");
			}
			else
				continue;

		/* the longest mount point contained in the expanded_filename wins */

		if (q - mount.mnt_mount > len) {
			len = q - mount.mnt_mount;
			strcpy(max_node, mount.mnt_node);
			strcpy(max_path, mount.mnt_path);
		}
	}

/* If the longest mount point was a local one, max_path is empty.
   Return FALSE, leaving node_name empty and expanded_filename as is.

   If the longest mount point is from a remote node, max_path
   contains the root of the file's path as it is known on the
   remote node.  Return TRUE, loading node_name with the remote
   node name and expanded_filename with the remote file name. */

	if (*max_path) {
		p = remote_filename;

		q = max_path;
		while (*q)
			*p++ = *q++;

		q = expanded_filename + len;
		while (*p++ = *q++);

		q = max_node;
		while (*node_name++ = *q++);

		p = remote_filename;
		while (*expanded_filename++ = *p++);

		flag = TRUE;
	}

#ifndef ultrix
#if (!defined AIX && !defined AIX_PPC)
	ib_fclose(mtab);
#endif
#endif

#ifdef hpux
	if (!flag)
		flag = get_server(expanded_filename, node_name);
#endif

#if (defined AIX || defined AIX_PPC)
	if (temp)
		gds__free(temp);
#endif

	return flag;
}



ISC_ast_enter()
{
/**************************************
 *
 *	I S C _ a s t _ e n t e r
 *
 **************************************
 *
 * Functional description
 *	Enter ast mode.
 *
 **************************************/

	++ast_count;
}


ISC_ast_exit()
{
/**************************************
 *
 *	I S C _ a s t _ e x i t
 *
 **************************************
 *
 * Functional description
 *	Enter ast mode.
 *
 **************************************/

	--ast_count;
}



ISC_enter()
{
/**************************************
 *
 *	I S C _ e n t e r
 *
 **************************************
 *
 * Functional description
 *	Enter ISC world from caller.
 *
 **************************************/
	SIGVEC internal_sigfpe;

	internal_sigfpe.sv_handler = (SIG_FPTR) signal_handler;
	internal_sigfpe.sv_mask = 0;
	internal_sigfpe.sv_onstack = 0;
	sigvector(SIGFPE, &internal_sigfpe, &client_sigfpe);
}


ISC_enable()
{
/**************************************
 *
 *	I S C _ e n a b l e
 *
 **************************************
 *
 * Functional description
 *	Enable signal processing.  Re-post and pending signals.
 *
 **************************************/
	USHORT n;
	SSHORT signal_number;

	if (inhibit_count)
		--inhibit_count;

	if (inhibit_count)
		return;

#ifdef UNIX
	while (pending_signals)
		for (n = 0; pending_signals && n < 32; n++)
			if (pending_signals & (1 << n)) {
				pending_signals &= ~(1 << n);
				ISC_kill(process_id, n + 1);
			}
#endif
}


ISC_event_blocked(count, events, values)
	 USHORT count;
	 SLONG **events;
	 SLONG *values;
{
/**************************************
 *
 *	I S C _ e v e n t _ b l o c k e d	( G E N E R I C )
 *
 **************************************
 *
 * Functional description
 *	If a wait would block, return TRUE.
 *
 **************************************/
}


SLONG ISC_event_clear(event)
	 SLONG *event;
{
/**************************************
 *
 *	I S C _ e v e n t _ c l e a r	( G E N E R I C )
 *
 **************************************
 *
 * Functional description
 *	Clear an event preparatory to waiting on it.  The order of
 *	battle for event synchronization is:
 *
 *	    1.  Clear event.
 *	    2.  Test data structure for event already completed
 *	    3.  Wait on event.
 *
 **************************************/
}


ISC_event_init(event)
	 SLONG *event;
{
/**************************************
 *
 *	I S C _ e v e n t _ i n i t	( G E N E R I C )
 *
 **************************************
 *
 * Functional description
 *	Prepare an event object for use.  Return FALSE if not
 *	supported.
 *
 **************************************/

	return FALSE;
}


ISC_event_post(event)
	 SLONG *event;
{
/**************************************
 *
 *	I S C _ e v e n t _ p o s t	( G E N E R I C )
 *
 **************************************
 *
 * Functional description
 *	Post an event to wake somebody else up.
 *
 **************************************/
}


ISC_event_wait(count, events, values, micro_seconds, timeout_handler,
			   handler_arg)
	 USHORT count;
	 SLONG **events;
	 SLONG *values;
	 int micro_seconds;
	 void (*timeout_handler) ();
	 void *handler_arg;
{
/**************************************
 *
 *	I S C _ e v e n t _ w a i t	( G E N E R I C )
 *
 **************************************
 *
 * Functional description
 *	Wait on an event.
 *
 **************************************/
}



ISC_exit()
{
/**************************************
 *
 *	I S C _ e x i t
 *
 **************************************
 *
 * Functional description
 *	Enter ISC world from caller.
 *
 **************************************/
	SIGVEC internal_sigfpe;

#ifndef ISC_EXIT
	sigvector(SIGFPE, &client_sigfpe, 0);
#endif
}


ISC_expand_filename(from_buff, length, to_buff)
	 TEXT *from_buff;
	 USHORT length;
	 TEXT *to_buff;
{
/**************************************
 *
 *	I S C _ e x p a n d _ f i l e n a m e		( N F S )
 *
 **************************************
 *
 * Functional description
 *	Expand a filename by following links.  As soon as a TCP node name
 *	shows up, stop translating.
 *
 **************************************/

	return expand_filename2(from_buff, length, to_buff);
}



#ifndef GET_HOST
TEXT *ISC_get_host(string, length)
	 TEXT *string;
	 USHORT length;
{
/**************************************
 *
 *	I S C _ g e t _ h o s t			( U N I X )
 *
 **************************************
 *
 * Functional description
 *	Get host name.
 *
 **************************************/

	gethostname(string, length);

	return string;
}
#endif


ISC_get_user(name, id, group, project, organization, node, user_string,
			 user_len)
	 TEXT *name;
	 int *id;
	 int *group;
	 TEXT *project;
	 TEXT *organization;
	 int *node;
	 TEXT *user_string;
	 USHORT user_len;
{
/**************************************
 *
 *	I S C _ g e t _ u s e r   ( U N I X )
 *
 **************************************
 *
 * Functional description
 *	Find out who the user is.
 *
 **************************************/
	uid_t euid;
	TEXT *p, user_group[256];
	struct passwd *passwd;

	if (user_string && *user_string) {
		strcpy(user_group, user_string);
		for (p = user_group; *p && (*p != '.'); *p++);
		if (*p) {
			*p++ = 0;
			euid = (uid_t) atoi(p);
		}
		p = user_group;
	}
	else {
		euid = geteuid();
		passwd = getpwuid(euid);
		if (passwd)
			p = passwd->pw_name;
		else
			p = "";
		endpwent();
	}

	if (name)
		strcpy(name, p);

	if (id)
		*id = euid;

	if (group)
		*group = getegid();

	if (project)
		*project = 0;

	if (organization)
		*organization = 0;

	if (node)
		*node = 0;

	return (euid == 0);
}



ISC_inhibit()
{
/**************************************
 *
 *	I S C _ i n h i b i t
 *
 **************************************
 *
 * Functional description
 *	Inhibit process of signals.  Signals will be
 *	retained until signals are eventually re-enabled,
 *	then re-posted.
 *
 **************************************/

	++inhibit_count;
}


ISC_kill(pid, signal_number)
	 int pid;
	 int signal_number;
{
/**************************************
 *
 *	I S C _ k i l l		( u n i x )
 *
 **************************************
 *
 * Functional description
 *	Notify somebody else.
 *
 **************************************/
	SLONG msg[3];
	int status, pipes[2];
	TEXT process[MAXPATHLEN], arg[10];

	status = kill(pid, signal_number);

	if (!status || errno != EPERM)
		return status;

/* Process is there, but we don't have the privilege to
   send to him.  */

	if (!relay_pipe) {
		gds__prefix(process, GDS_RELAY);
		if (pipe(pipes)) {
			gds__log("ISC_kill: error %d creating gds_relay", errno);
			return -1;
		}
		sprintf(arg, "%d", pipes[0]);
		if (!vfork()) {
			execl(process, process, arg, 0);
			gds__log("ISC_kill: error %d starting gds_relay %s", errno,
					 process);
			_exit(0);
		}
		relay_pipe = pipes[1];

		/* Don't need the READ pipe */
		close(pipes[0]);
	}

	msg[0] = pid;
	msg[1] = signal_number;
	msg[2] = msg[0] ^ msg[1];
	if (write(relay_pipe, msg, sizeof(msg)) != sizeof(msg)) {
		gds__log("ISC_kill: write to relay_pipe failed %d", errno);
		relay_pipe = 0;			/* try to restart next time */
		return -1;
	}

	return 0;
}


ISC_make_user_string(name, namelen, group)
	 TEXT *name;
	 SSHORT namelen;
	 SLONG group;
{
/**************************************
 *
 *	I S C _ m a k e _ u s e r _ s t r i n g
 *
 **************************************
 *
 * Functional description
 *	Make a string from name and group.
 *
 **************************************/
	SCHAR temp[15];

	if (group) {
		sprintf(temp, ".%ld", group);
		strcat(name, temp);
	}
}
#endif



UCHAR *ISC_map_file(status_vector, filename, init_routine, init_arg, length,
					shmem_data)
	 STATUS *status_vector;
	 TEXT *filename;
	 int (*init_routine) ();
	 SLONG init_arg;
	 SLONG length;
	 void *shmem_data;
{
/**************************************
 *
 *	I S C _ m a p _ f i l e		( g e n e r i c )
 *
 **************************************
 *
 * Functional description
 *	Try to map a given file.  If we are the first (i.e. only)
 *	process to map the file, call a given initialization
 *	routine (if given) or punt (leaving the file unmapped).
 *
 **************************************/

	*status_vector++ = gds_arg_gds;
	*status_vector++ = gds__unavailable;
	*status_vector++ = gds_arg_end;

	return NULL;
}



ISC_mutex_init(mutex, dummy)
	 SLONG *mutex;
	 SLONG dummy;
{
/**************************************
 *
 *	I S C _ m u t e x _ i n i t	( G E N E R I C )
 *
 **************************************
 *
 * Functional description
 *	Initialize a mutex.
 *
 **************************************/

	return 0;
}


ISC_mutex_lock(mutex)
	 SLONG *mutex;
{
/**************************************
 *
 *	I S C _ m u t e x _ l o c k	( G E N E R I C )
 *
 **************************************
 *
 * Functional description
 *	Sieze a mutex.
 *
 **************************************/
	return 0;
}


ISC_mutex_unlock(mutex)
	 SLONG *mutex;
{
/**************************************
 *
 *	I S C _ m u t e x _ u n l o c k		( G E N E R I C )
 *
 **************************************
 *
 * Functional description
 *	Release a mutex.
 *
 **************************************/

	return 0;
}



ISC_set_user(string)
	 TEXT *string;
{
/**************************************
 *
 *	I S C _ s e t _ u s e r
 *
 **************************************
 *
 * Functional description
 *	Set user name for feeble operating system that don't
 *	support the concept, or support it badly.
 *
 **************************************/

	strcpy(user_name, string);
}


ISC_signal(signal_number, handler, arg)
	 int signal_number;
	 void (*handler) ();
	 SLONG arg;
{
/**************************************
 *
 *	I S C _ s i g n a l		( u n i x )
 *
 **************************************
 *
 * Functional description
 *	Multiplex multiple handers into single signal.
 *
 **************************************/
	SIGVEC vec, old_vec;
	SIG sig;
	SIG_FPTR ptr;

/* The signal handler needs the process id */
	if (!process_id)
		process_id = getpid();

/* See if this signal has ever been cared about before */
	for (sig = signals; sig; sig = sig->sig_next)
		if (sig->sig_signal == signal_number)
			break;

/* If it hasn't been (and its not FPE, since we won't chain
   handlers for FPE) attach our chain handler to the signal,
   and queue up whatever used to handle it as a non-ISC
   routine (the are invoked differently).  Note that if
   the old action was SIG_DFL, SIG_HOLD, SIG_IGN or our
   multiplexor, there is no need to save it. */
	if (!sig && signal_number != SIGFPE) {
		vec.sv_handler = (SIG_FPTR) signal_handler;
		vec.sv_mask = 0;
		vec.sv_onstack = 0;
		sigvector(signal_number, &vec, &old_vec);
		ptr = old_vec.sv_handler;
		if (ptr != SIG_DFL &&
#ifdef SIG_HOLD
			ptr != SIG_HOLD &&
#endif
			ptr != SIG_IGN && ptr != signal_handler)
			que_signal(signal_number, ptr, arg, SIG_client);
	}

/* Que up the new ISC signal handler routine */
	que_signal(signal_number, handler, arg, 0);
}



ISC_signal_cancel(signal_number, handler, arg)
	 int signal_number;
	 SIG_FPTR handler;
	 SLONG arg;
{
/**************************************
 *
 *	I S C _ s i g n a l _ c a n c e l
 *
 **************************************
 *
 * Functional description
 *	Cancel a signal handler.
 *
 **************************************/
	SIG sig, *ptr;

	for (ptr = &signals; sig = *ptr; ptr = &(*ptr)->sig_next)
		if (sig->sig_signal == signal_number &&
			sig->sig_routine == handler && sig->sig_arg == arg) {
			*ptr = sig->sig_next;
			gds__free(sig);
			break;
		}
}


ISC_signal_init()
{
/**************************************
 *
 *	I S C _ s i g n a l _ i n i t
 *
 **************************************
 *
 * Functional description
 *	Initialize any system signal handlers.
 *
 **************************************/

	overflow_count = 0;
	gds__register_cleanup(cleanup, 0);
	ISC_signal(SIGFPE, overflow_handler, 0);
}


static cleanup()
{
/**************************************
 *
 *	c l e a n u p
 *
 **************************************
 *
 * Functional description
 *	Module level cleanup handler.
 *
 **************************************/

	signals = NULL;
	inhibit_count = 0;
	process_id = 0;
}


static error(status_vector, string, status)
	 STATUS *status_vector;
	 TEXT *string;
	 STATUS status;
{
/**************************************
 *
 *	e r r o r
 *
 **************************************
 *
 * Functional description
 *	We've encountered an error, report it.
 *
 **************************************/

	*status_vector++ = gds_arg_gds;
	*status_vector++ = gds__sys_request;
	*status_vector++ = gds_arg_string;
	*status_vector++ = (STATUS) string;
	*status_vector++ = SYS_ARG;
	*status_vector++ = status;
	*status_vector++ = gds_arg_end;
}


static expand_filename2(from_buff, length, to_buff)
	 TEXT *from_buff;
	 USHORT length;
	 TEXT *to_buff;
{
/**************************************
 *
 *	e x p a n d _ f i l e n a m e 2		( N F S )
 *
 **************************************
 *
 * Functional description
 *	Expand a filename by following links.  As soon as a TCP node name
 *	shows up, stop translating.
 *
 **************************************/
	TEXT *from, *to, temp[MAXPATHLEN], *p, *segment, temp2[MAXPATHLEN];
	SSHORT n;
	struct passwd *passwd;

	if (length) {
		strncpy(temp2, from_buff, length);
		temp2[length] = 0;
		from = temp2;
	}
	else
		from = from_buff;

	to = to_buff;

/* If the filename contains a TCP node name, don't even try to expand it */

	if (strchr(from, INET_FLAG)) {
		strcpy(to, from);
		return strlen(to);
	}

/* Handle references to default directories (tilde refs) */

	if (*from == '~') {
		++from;
		p = temp;
		while (*from && *from != '/')
			*p++ = *from++;
		*p = 0;
		passwd = (temp[0]) ? getpwnam(temp) : getpwuid(geteuid());
		if (passwd) {
			expand_filename2(passwd->pw_dir, 0, temp);
			p = temp;
			while (*p)
				*to++ = *p++;
		}
	}

/* If the file is local, expand partial pathnames with default directory */

	if (*from && !strchr(from, INET_FLAG) && *from != '/' && getwd(to)) {
		while (*to)
			++to;
		*to++ = '/';
	}

/* Process file name segment by segment looking for symbolic
   links. See ISC_analyze_nfs for how NFS mount points are
   handled. */

	while (*from) {
		segment = to;

		/* Copy the leading slash, if any */

		if (*from == '/') {
			if (to > to_buff + 1 && to[-1] == '/')
				++from;
			else
				*to++ = *from++;
			continue;
		}

		/* Handle self references */

		if (*from == '.' && (from[1] == '.' || from[1] == '/')) {
			++from;
			if (*from == '.') {
				++from;
				if (to > to_buff)
					--to;
				while (to > to_buff && to[-1] != '/')
					--to;
			}
			continue;
		}

		/* Copy the rest of the segment name */

		while (*from && *from != '/')
			*to++ = *from++;

		/* If the file is local, check for a symbol link */

		*to = 0;

		n = readlink(to_buff, temp, sizeof(temp));

		if (n < 0)
			continue;

		/* We've got a link.  If it contains a node name or it starts 
		   with a slash, it replaces the initial segment so far */

		temp[n] = 0;
		p = temp;

		if (strchr(temp, INET_FLAG)) {
			strcpy(to_buff, temp);
			return n;
		}

		to = (*p == '/') ? to_buff : segment;

		while (*p)
			*to++ = *p++;

		/* Whole link needs translating -- recurse */

		*to = 0;
		expand_filename2(to_buff, 0, temp);
		to = to_buff;
		p = temp;
		while (*p)
			*to++ = *p++;
	}

	*to = 0;

	return to - to_buff;
}




#ifdef hpux
static get_mounts(mount, buffer, file)
	 MNT *mount;
	 TEXT *buffer;
	 IB_FILE *file;
{
/**************************************
 *
 *	g e t _ m o u n t s	( H P - U X )
 *
 **************************************
 *
 * Functional description
 *	Get ALL mount points.
 *
 **************************************/
	TEXT *p, *q;
	struct mntent *mptr;

/* Start by finding a mount point. */

	p = buffer;

	while ((mptr = getmntent(file)) > 0) {
		/* Include non-NFS (local) mounts - some may be SLONGer than
		   NFS mount points */

/****
    if (strcmp (mptr->mnt_type, MNTTYPE_NFS))
	continue;
****/

		mount->mnt_node = p;
		q = mptr->mnt_fsname;
		while (*q && *q != ':')
			*p++ = *q++;
		*p++ = 0;
		if (*q)
			q++;
		mount->mnt_path = p;
		while (*p++ = *q++);
		mount->mnt_mount = mptr->mnt_dir;
		return TRUE;
	}

	return FALSE;
}
#endif


#if (defined AIX || defined AIX_PPC)
static get_mounts(mount, mnt_buffer, buffer, count)
	 MNT *mount;
	 TEXT *mnt_buffer;
	 TEXT **buffer;
	 int *count;
{
/**************************************
 *
 *	g e t _ m o u n t s	( A I X )
 *
 **************************************
 *
 * Functional description
 *	Get ALL mount points.
 *
 **************************************/
	int l, i;
	TEXT *p;
	struct vmount *vmt;

	if (!*buffer) {
		/* The first time through, get the mount info from the system.
		   First find out how much information there is, then allocate
		   a buffer for it, and finally get it. */

		if (mntctl(MCTL_QUERY, sizeof(SLONG), mnt_buffer) != 0)
			return FALSE;

		l = *(SLONG *) mnt_buffer;
		/* FREE: in get_mounts() */
		if (!(*buffer = gds__alloc(l)) ||
			(*count = mntctl(MCTL_QUERY, l, *buffer)) <= 0)
			return FALSE;		/* NOMEM: */
	}
	else if (!*count)
		return FALSE;

	for (i = --(*count), p = *buffer; i--;)
		p += ((struct vmount *) p)->vmt_length;

	vmt = (struct vmount *) p;

	mount->mnt_node = mnt_buffer;
	p = vmt2dataptr(vmt, VMT_HOSTNAME);
	l = vmt2datasize(vmt, VMT_HOSTNAME);
	if (l && (p[0] != '-' || p[1]))
		while (l-- && *p)
			*mnt_buffer++ = *p++;
	*mnt_buffer++ = 0;

	mount->mnt_path = mnt_buffer;
	p = vmt2dataptr(vmt, VMT_OBJECT);
	l = vmt2datasize(vmt, VMT_OBJECT);
	while (l-- && *p)
		*mnt_buffer++ = *p++;
	*mnt_buffer++ = 0;

	mount->mnt_mount = mnt_buffer;
	p = vmt2dataptr(vmt, VMT_STUB);
	l = vmt2datasize(vmt, VMT_STUB);
	while (l-- && *p)
		*mnt_buffer++ = *p++;
	*mnt_buffer = 0;

	if (!*mount->mnt_node) {
		p = mount->mnt_node;
		mount->mnt_node = mount->mnt_path;
		mount->mnt_path = p;
	}

	return TRUE;
}
#endif


#ifndef hpux
#if (!defined AIX && !defined AIX_PPC)
static get_mounts(mount, buffer, file)
	 MNT *mount;
	 TEXT *buffer;
	 IB_FILE *file;
{
/**************************************
 *
 *	g e t _ m o u n t s	( n o n - U l t r i x )
 *
 **************************************
 *
 * Functional description
 *	Get ALL mount points.
 *
 **************************************/
	TEXT device[128], mount_point[128], type[16], rw[128], foo1[16], *p, *q;
	SSHORT n;

/* Start by finding a mount point. */

	p = buffer;

	for (;;) {
		n =
			ib_fscanf(file, "%s %s %s %s %s %s", device, mount_point, type,
					  rw, foo1, foo1);
		if (n < 0)
			break;

		/* Include non-NFS (local) mounts - some may be SLONGer than
		   NFS mount points */

/****
    if (strcmp (type, "nfs"))
	continue;
****/

		mount->mnt_node = p;
		q = device;
		while (*q && *q != ':')
			*p++ = *q++;
		*p++ = 0;
		if (*q)
			q++;
		mount->mnt_path = p;
		while (*p++ = *q++);
		mount->mnt_mount = p;
		q = mount_point;
		while (*p++ = *q++);
		return TRUE;
	}

	return FALSE;
}
#endif
#endif


static overflow_handler()
{
/**************************************
 *
 *	o v e r f l o w _ h a n d l e r
 *
 **************************************
 *
 * Functional description
 *	Somebody overflowed.  Ho hum.
 *
 **************************************/

	++overflow_count;
}


static SIG que_signal(signal_number, handler, arg, flags)
	 int signal_number;
	 void (*handler) ();
	 SLONG arg;
	 int flags;
{
/**************************************
 *
 *	q u e _ s i g n a l
 *
 **************************************
 *
 * Functional description
 *	Que signal for later action.
 *
 **************************************/
	SIG sig;

	sig = (SIG) gds__alloc((SLONG) sizeof(struct sig));
/* FREE: unknown */
	if (!sig)					/* NOMEM: */
		return NULL;			/* Error not really handled */
	sig->sig_signal = signal_number;
	sig->sig_routine = handler;
	sig->sig_arg = arg;
	sig->sig_flags = flags;
	sig->sig_count = 0;

	sig->sig_next = signals;
	signals = sig;

	return sig;
}



static void signal_handler(number, code, scp)
	 int number;
	 int code;
	 struct sigcontext *scp;
{
/**************************************
 *
 *	s i g n a l _ h a n d l e r	( G E N E R I C )
 *
 **************************************
 *
 * Functional description
 *	Checkin with various signal handlers.
 *
 **************************************/
	SIG sig;

/* If signals are inhibited, save the signal for later reposting.
   Otherwise, invoked everybody who may have express an interest. */

	if (inhibit_count)
		pending_signals |= 1 << (number - 1);
	else
		for (sig = signals; sig; sig = sig->sig_next)
			if (sig->sig_signal == number) {
				if (sig->sig_flags & SIG_client)
					(*sig->sig_routine) (number, code, scp);
				else
					(*sig->sig_routine) (sig->sig_arg);
			}

#ifdef SIG_RESTART
	scp->sc_syscall_action = (blocked) ? SIG_RETURN : SIG_RESTART;
#endif
}
