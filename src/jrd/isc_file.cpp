/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		isc_file.c
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
 * 2001.06.14: Claudio Valderrama: Possible buffer overrun in
 *	expand_share_name(TEXT*) has been closed. Parameter is return value, too.
 *	This function and its caller in this same file don't report error conditions.
 * 2002.02.15 Sean Leyne - Code Cleanup, removed obsolete "EPSON" port
 * 2002.02.15 Sean Leyne - Code Cleanup, removed obsolete "DELTA" port
 * 2002-02-23 Sean Leyne - Code Cleanup, removed old M88K and NCR3000 port
 */

#ifdef SHLIB_DEFS
#define LOCAL_SHLIB_DEFS
#endif

#include "firebird.h"
#include "../jrd/ib_stdio.h"
#include <stdlib.h>
#include <string.h>
#include "../jrd/common.h"
#include "gen/codes.h"
#include "../jrd/jrd.h"
#include "../jrd/flu_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/isc_proto.h"
#include "../jrd/isc_f_proto.h"
#include "../jrd/jrd_proto.h"

#ifndef NO_NFS
#define NFS	1
#endif

#ifdef SUN3_3
typedef unsigned short uid_t;
#endif


/* VMS Specific Stuff */

#ifdef VMS

#include <rms.h>
#include <descrip.h>
#include <ssdef.h>
#include <jpidef.h>
#include <prvdef.h>
#include <secdef.h>
#include <lckdef.h>
#include "../jrd/lnmdef.h"
#define LOGICAL_NAME_TABLE	"LNM$FILE_DEV"
#define DEFAULT_FILE_NAME	".gdb"
#define INET_FLAG		'^'

typedef struct itm {
	SSHORT itm_length;
	SSHORT itm_code;
	SCHAR *itm_buffer;
	SSHORT *itm_return_length;
} ITM;

#endif /* of ifdef VMS */


/* Unix/NFS specific stuff */

#if (defined NFS || defined FREEBSD || defined NETBSD || defined SINIXZ)
#include <pwd.h>
#define MAXPATHLEN	1024
#endif

#ifdef hpux
#define GETWD(buf)		getcwd (buf, MAXPATHLEN)
#define MTAB			"/etc/mnttab"
#include <sys/types.h>
/* RITTER - added HP11 to the pre-processor condition below */
#if !(defined HP10 || defined HP11)
#include <cluster.h>
#endif
#endif

#ifdef DGUX
#define MTAB			"/etc/mnttab"
#endif

#if (defined SOLARIS || defined UNIXWARE)
#define SV_MNTENT
#define NON_MNTENT
#include <sys/mnttab.h>
#define MTAB			"/etc/mnttab"
#define MTAB_OPEN(path,type)    ib_fopen (path, type)
#define MTAB_CLOSE(stream)	ib_fclose (stream)
#endif

#ifdef DARWIN
#define MTAB			"/etc/fstab.hd"
#define MTAB_OPEN(path,type)	ib_fopen (path, type)
#define MTAB_CLOSE(stream)	ib_fclose (stream)
#endif

#ifdef SCO_UNIX
#define NON_MNTENT
#define MTAB			"/etc/mount"
#define MTAB_OPEN(path,type)	popen (path, type)
#define MTAB_CLOSE(stream)	pclose (stream)
#define GETWD(buf)		getcwd (buf, MAXPATHLEN)
#endif

#ifdef DECOSF
#define NON_MNTENT
#include <sys/types.h>
#include <sys/mount.h>
#endif

#ifdef SINIXZ
#include <sys/mnttab.h>
#define SV_MNTENT
#define NON_MNTENT
#define MTAB			"/etc/mnttab"
#define MTAB_OPEN(path,type)	fopen (path, type)
#define MTAB_CLOSE(stream)	fclose (stream)
#endif

#ifdef ultrix
#define NON_MNTENT
#include <sys/mount.h>
#include <sys/fs_types.h>
#endif

#if (defined AIX || defined AIX_PPC)
#define NON_MNTENT
#include <sys/vmount.h>
#endif

#if (defined SOLARIS || defined UNIXWARE || defined linux || defined FREEBSD || defined NETBSD || defined DARWIN || defined SINIXZ)
#define GETWD(buf)		getcwd (buf, MAXPATHLEN)
#endif

#ifdef SUPERSERVER
#undef GETWD
#define GETWD(buf)		JRD_getdir (buf, MAXPATHLEN)
#endif

#ifndef GETWD
#define GETWD			getwd
#endif

#ifndef NON_MNTENT
#ifdef NFS
#include <mntent.h>
#define MTAB_OPEN(path,type)	setmntent (path, "r")
#define MTAB_CLOSE(stream)	endmntent (stream)
#endif
#endif

#ifndef MTAB
#define MTAB			"/etc/mtab"
#endif

typedef struct mnt {
	TEXT *mnt_node;
	TEXT *mnt_mount;
	TEXT *mnt_path;
} MNT;


/* Unix specific stuff */

#ifdef UNIX
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/file.h>
#include <errno.h>

#ifdef DARWIN
#include </usr/include/pwd.h>
#else
extern struct passwd *getpwnam(), *getpwuid();
#endif
#endif	// UNIX


/* MS/DOS / OS2 / NLM Junk */

#ifdef PC_PLATFORM
#include <stdlib.h>

#define PARSE_FILENAME
#define ISC_DATABASE	"ISC_DATABASE"	/* default database directory
										   environment variable */

/* definition of toupper () */
#ifndef __CTYPE_H
#include <ctype.h>
#endif /* __CTYPE_H */

#endif /* PC_PLATFORM */

#ifdef NETWARE_386
#define PARSE_FILENAME
#endif

/* Windows NT stuff */

#ifdef WIN_NT
#include <windows.h>
#ifndef TEXT
#define TEXT		SCHAR
#endif

#ifndef MAXPATHLEN
#define MAXPATHLEN	_MAX_PATH
#endif
#endif


#ifndef MAXPATHLEN
#define MAXPATHLEN	512
#endif

#ifndef MAXHOSTLEN
#define MAXHOSTLEN	64
#endif

#ifndef INET_FLAG
#define INET_FLAG	':'
#endif

static int expand_filename2(TEXT *, USHORT, TEXT *);

#if defined(WIN_NT)
static void expand_share_name(TEXT *);
static void share_name_from_resource(TEXT *, TEXT *, LPNETRESOURCE);
static void share_name_from_unc(TEXT *, TEXT *, LPREMOTE_NAME_INFO);
#endif

#if (defined AIX || defined AIX_PPC || defined DECOSF)
static BOOLEAN get_mounts(MNT *, TEXT *, TEXT **, int *);
#else
#ifdef ultrix
static BOOLEAN get_mounts(MNT *, TEXT *, int *);
#else
static BOOLEAN get_mounts(MNT *, TEXT *, IB_FILE *);
#endif
#endif
static BOOLEAN get_server(TEXT *, TEXT *);
#if (defined SPX) || ((defined PC_PLATFORM) && !(defined NETWARE_386))
static void string_parse(TEXT *, TEXT **, TEXT **, TEXT **, TEXT *);
static void convert_slashes(char oldslash, char newslash, char *string);
#endif


#ifdef SHLIB_DEFS
#define strlen		(*_libgds_strlen)
#define strcmp		(*_libgds_strcmp)
#define strcpy		(*_libgds_strcpy)
#define _iob		(*_libgds__iob)
#define errno		(*_libgds_errno)
#define ib_fopen		(*_libgds_fopen)
#define ib_fclose		(*_libgds_fclose)
#define open		(*_libgds_open)
#define strcat		(*_libgds_strcat)
#define strncpy		(*_libgds_strncpy)
#define strchr		(*_libgds_strchr)
#define getpwnam	(*_libgds_getpwnam)
#define geteuid		(*_libgds_geteuid)
#define getpwuid	(*_libgds_getpwuid)
#define close		(*_libgds_close)
#define ib_fscanf		(*_libgds_fscanf)
#define _ctype		(*_libgds__ctype)
#define readlink	(*_libgds_readlink)

extern int strlen();
extern int strcmp();
extern SCHAR *strcpy();
extern IB_FILE _iob[];
extern int errno;
extern IB_FILE *ib_fopen();
extern int ib_fclose();
extern int open();
extern SCHAR *strcat();
extern SCHAR *strncpy();
extern SCHAR *strchr();
extern struct passwd *getpwnam();
extern uid_t geteuid();
extern struct passwd *getpwuid();
extern int close();
extern int ib_fscanf();
extern SCHAR _ctype[];
extern int readlink();

#ifndef SCO
#define GETWD		(*_libgds_getwd)
#define setmntent	(*_libgds_setmntent)
#define getmntent	(*_libgds_getmntent)
#define endmntent	(*_libgds_endmntent)

extern SCHAR *GETWD();
extern IB_FILE *setmntent();
extern struct mntent *getmntent();
extern int endmntent();
#else
#define getcwd		(*_libgds_getcwd)
#define popen		(*_libgds_popen)
#define pclose		(*_libgds_pclose)

extern IB_FILE *popen();
extern IB_FILE *pclose();
extern SCHAR *getcwd();
#endif
#endif


#ifdef NFS
int ISC_analyze_nfs(TEXT * expanded_filename, TEXT * node_name)
{
/**************************************
 *
 *	I S C _ a n a l y z e _ n f s
 *
 **************************************
 *
 * Functional description
 *	Check a file name for an NFS mount point.  If so,
 *	decompose into node name and remote file name.
 *
 **************************************/
#ifdef STACK_EFFICIENT
	TEXT *mnt_buffer, *remote_filename, *max_node, *max_path, *expand_mount;
#else /* STACK_EFFICIENT */
	TEXT mnt_buffer[BUFFER_LARGE], remote_filename[MAXPATHLEN],
		max_node[MAXHOSTLEN], max_path[MAXPATHLEN], expand_mount[MAXPATHLEN];
#endif /* STACK_EFFICIENT */

	MNT mount;
	TEXT *p, *q, *temp;
	USHORT flag;

	IB_FILE *mtab;
	int context, len;

#ifdef STACK_EFFICIENT
/* allocate all strings in one block */
	mnt_buffer = (TEXT *)
		gds__alloc((SLONG)
				   (sizeof(TEXT) *
					(BUFFER_LARGE + 3 * MAXPATHLEN + MAXHOSTLEN)));
	remote_filename = mnt_buffer + BUFFER_LARGE;
	max_node = remote_filename + MAXPATHLEN;
	max_path = max_node + MAXHOSTLEN;
	expand_mount = max_path + MAXPATHLEN;
#endif /* STACK_EFFICIENT */

	len = 0;
	*max_path = 0;
	flag = FALSE;

/* Search mount points */

	temp = NULL;
	context = 0;

#ifdef ultrix
	while (get_mounts(&mount, mnt_buffer, &context))
#else
#if (defined AIX || defined AIX_PPC || defined DECOSF)
	while (get_mounts(&mount, mnt_buffer, &temp, &context))
#else
	if (!(mtab = MTAB_OPEN(MTAB, "r"))) {
#ifdef STACK_EFFICIENT
		gds__free((SLONG *) mnt_buffer);
#endif /* STACK_EFFICIENT */
		return flag;
	}
	while (get_mounts(&mount, mnt_buffer, mtab))
#endif
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

		if (q - mount.mnt_mount >= len) {
			len = q - mount.mnt_mount;
			if (mount.mnt_node) {
				strcpy(max_node, mount.mnt_node);
				strcpy(max_path, mount.mnt_path);
			}
			else {
				*max_node = 0;
				*max_path = 0;
			}
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
#ifndef DECOSF
	MTAB_CLOSE(mtab);
#endif
#endif
#endif

#ifdef hpux
/* RITTER - added HP11 to the pre-processor condition below */
#if !(defined HP10 || defined HP11)
	if (!flag)
		flag = get_server(expanded_filename, node_name);
#endif
#endif

#if (defined AIX || defined AIX_PPC || defined DECOSF)
	if (temp)
		gds__free(temp);
#endif

#ifdef STACK_EFFICIENT
	gds__free((SLONG *) mnt_buffer);
#endif /* STACK_EFFICIENT */

	return flag;
}
#endif


#if defined(WIN_NT)
int ISC_analyze_pclan(TEXT * expanded_name, TEXT * node_name)
{
/**************************************
 *
 *	I S C _ a n a l y z e _ p c l a n
 *
 **************************************
 *
 * Functional description
 *	Analyze a filename for a named pipe node name on the front.
 *	If one is found, extract the node name, compute the residual
 *	file name, and return TRUE.  Otherwise return FALSE.
 *
 **************************************/
	TEXT *p, *q, localhost[64];

	if ((expanded_name[0] != '\\' && expanded_name[0] != '/') ||
		(expanded_name[1] != '\\' && expanded_name[1] != '/'))
		return FALSE;

	p = node_name;
	*p++ = '\\';
	*p++ = '\\';
	for (q = expanded_name + 2; *q && *q != '\\' && *q != '/';)
		*p++ = *q++;
	*p = 0;

/* If a drive letter or TCP node name follows the slash after the
   named pipe node name, space over the slash. */

	if ((*q == '\\' || *q == '/') && (p = strpbrk(q + 1, ":\\/"))
		&& *p == ':')
		q++;

	for (p = expanded_name; *p++ = *q++;);

/* If this is a loopback, substitute "." for the host name.  Otherwise,
   the CreateFile on the pipe will fail. */

	ISC_get_host(localhost, sizeof(localhost));
	if (!stricmp(node_name + 2, localhost)) {
		node_name[2] = '.';
		node_name[3] = 0;
	}

	return TRUE;
}
#endif	// WIN_NT


int DLL_EXPORT ISC_analyze_spx(TEXT * expanded_name, TEXT * node_name)
{
/**************************************
 *
 *	I S C _ a n a l y z e _ s p x
 *
 **************************************
 *
 * Functional description
 *	Analyze a filename for a SPX separator.
 *	If one is found, extract the node name, compute the residual
 *	file name, and return TRUE.  Otherwise return FALSE.
 *
 **************************************/
	TEXT *p;

	p = expanded_name;

/* Scan looking for the SPX node separator character (node@path).
   Also check for an INET separator before the SPX separator in case
   this is a network hop (e.g. tcp_node:spx_node@path). */

	for (p = expanded_name; *p && *p != '@' && *p != INET_FLAG; p++);

/* If we didn't find an SPX separator or found a TCP one first, return. */

	if (!*p || *p == INET_FLAG)
		return FALSE;

/* We found a valid SPX separator.  Copy left of separator into node_name
   output variable & increment p to point to remaining path. */

	*p++ = 0;
	strcpy(node_name, expanded_name);

/* Shift the filename over in the string to strip off the node name */

	while (*expanded_name++ = *(p++));

	return TRUE;
}


int DLL_EXPORT ISC_analyze_tcp(TEXT * file_name, TEXT * node_name)
{
/**************************************
 *
 *	I S C _ a n a l y z e _ t c p		( G E N E R I C )
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
	USHORT dtype;

/* Scan file name looking for separator character */

	if (!(p = strchr(file_name, INET_FLAG)))
		return FALSE;

#ifdef NETWARE_386
/* Filename for NetWare is of form node:[/][vol:][dir/../]filename.ext
   so if only one ':' is found then path does not contain a node.
   One has already been found, look for another. */

	if (!strchr(p + 1, ':'))
		return FALSE;

#else
#if (defined PC_PLATFORM)
/* for DOS and OS/2, introduce a restriction against one-character
   machine names as a kludge to prevent the situation of
   trying to attach to C: as a remote machine -- there has
   got to be a better way to resolve this */

	if (p - file_name == 1)
		return FALSE;
#endif
#endif

#ifdef WIN_NT
/* For Windows NT, insure that a single character node name does
   not conflict with an existing drive letter. */

	if (p - file_name == 1) {
		*node_name = *file_name;
		strcpy(node_name + 1, ":\\");
		dtype = GetDriveType(node_name);
		if (dtype > 1 && dtype != DRIVE_REMOTE)
			return FALSE;
	}
#endif

	*p = 0;
	strcpy(node_name, file_name);

#ifdef NETWARE_386
/* for NetWare [node:] may be followed by a '\' or a '/' if so
   do not include it in file name. */

	if ((*(p + 1) == '/') || (*(p + 1) == '\\'))
		p++;
#endif

	while (*file_name++ = *++p);

	return TRUE;
}


BOOLEAN DLL_EXPORT ISC_check_if_remote(TEXT * file_name,
									   BOOLEAN implicit_flag)
{
/**************************************
 *
 *	I S C _ c h e c k _ i f _ r e m o t e
 *
 **************************************
 *
 * Functional description
 *	Check to see if a path name resolves to a
 *	remote file.  If implicit_flag is TRUE, then
 *	analyze the path to see if it resolves to a
 *	file on a remote machine.  Otherwise, simply
 *	check for an explicit node name.
 *
 **************************************/
#ifdef STACK_EFFICIENT
	TEXT *temp_name;
#else /* STACK_EFFICIENT */
	TEXT temp_name[MAXPATHLEN];
#endif /* STACK_EFFICIENT */
	TEXT host_name[64];

#ifdef STACK_EFFICIENT
	temp_name = (TEXT *) gds__alloc((SLONG) (sizeof(TEXT) * MAXPATHLEN));
#endif /* STACK_EFFICIENT */

	strcpy(temp_name, file_name);

/* Always check for an explicit TCP node name */

	if (ISC_analyze_tcp(temp_name, host_name)) {
#ifdef STACK_EFFICIENT
		gds__free((SLONG *) temp_name);
#endif /* STACK_EFFICIENT */

		return TRUE;
	}
#ifdef NFS
	if (implicit_flag) {
		/* Check for a file on an NFS mounted device */

		if (ISC_analyze_nfs(temp_name, host_name)) {
#ifdef STACK_EFFICIENT
			gds__free((SLONG *) temp_name);
#endif /* STACK_EFFICIENT */

			return TRUE;
		}
	}
#endif

#if defined(WIN_NT)
/* Check for an explicit named pipe node name */

	if (ISC_analyze_pclan(temp_name, host_name)) {
#ifdef STACK_EFFICIENT
		gds__free((SLONG *) temp_name);
#endif /* STACK_EFFICIENT */

		return TRUE;
	}

	if (implicit_flag) {
#ifdef STACK_EFFICIENT
		TEXT *temp_name2;
		temp_name2 = (TEXT *) gds__alloc((SLONG) (sizeof(TEXT) * MAXPATHLEN));
#else /* STACK_EFFICIENT */
		TEXT temp_name2[MAXPATHLEN];
#endif /* STACK_EFFICIENT */

		/* Check for a file on a shared drive.  First try to expand
		   the path.  Then check the expanded path for a TCP,
		   named pipe or SPX node name. */

		ISC_expand_share(temp_name, temp_name2);
		if (ISC_analyze_tcp(temp_name2, host_name) ||
			ISC_analyze_pclan(temp_name2, host_name) ||
			ISC_analyze_spx(temp_name2, host_name)) {
#ifdef STACK_EFFICIENT
			gds__free((SLONG *) temp_name2);
			gds__free((SLONG *) temp_name);
#endif /* STACK_EFFICIENT */

			return TRUE;
		}

#ifdef STACK_EFFICIENT
		gds__free((SLONG *) temp_name2);
#endif /* STACK_EFFICIENT */

	}
#endif	// WIN_NT

#ifdef STACK_EFFICIENT
	gds__free((SLONG *) temp_name);
#endif /* STACK_EFFICIENT */

	return FALSE;
}


#if (defined NFS || defined FREEBSD || defined NETBSD || defined SINIXZ)
int ISC_expand_filename(TEXT * from_buff, USHORT length, TEXT * to_buff)
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
#endif


#if (defined PC_PLATFORM) && !(defined NETWARE_386)
int ISC_expand_filename(
						TEXT * file_name,
						USHORT file_length, TEXT * expanded_name)
{
/**************************************
 *
 *	I S C _ e x p a n d _ f i l e n a m e	( P C _ P L A T F O R M )
 *
 **************************************
 *
 * Functional description
 *      This looks for patterns in the file name like:
 *
 *          host:[\|/]path[\|/]          - TCP/IP node
 *          host:vol:[\|/]path[\|/]      - Netware node
 *          host@vol[:][\|/]path[\|/]    - SPX node
 *
 *      If no host is specified, it checks for an ISC_DATABASE
 *      environment variable and uses that.
 *      If there is no slash after the node name, one is inserted.
 *      If there is no trailing slash after an ISC_DATABASE path,
 *      one is inserted.
 *
 *      The final result is an expanded path name.
 *
 **************************************/
	TEXT c, *p, *in, *out, *default_directory, *colon, *colon2, *atsign;

#ifdef STACK_EFFICIENT
	TEXT *local = (TEXT *) gds__alloc((SLONG) (sizeof(TEXT) * MAXPATHLEN));
#else /* STACK_EFFICIENT */
	TEXT local[MAXPATHLEN];
#endif /* STACK_EFFICIENT */

/* if file_length is specified, make sure the string is null terminated */

	if (!file_length)
		file_length = strlen(file_name);

	strncpy(local, file_name, file_length);
	local[file_length] = 0;
	in = local;
	out = expanded_name;

/* If there is an explicit node name of the form \\DOPEY or //DOPEY
   assume named pipes.  Translate forward slashes to back slashes
   and return with no further processing. */

	if ((in[0] == '\\' && in[1] == '\\') || (in[0] == '/' && in[1] == '/')) {
		strcpy(out, in);

		/* Translate forward slashes to back slashes */
		convert_slashes('/', '\\', out);
		return file_length;
	}

/* do some parsing of the path to look for colons (nodes), two colons
   (netware nodes), an at sign and a colon (spx), slashes and backslashes */

	c = 0;
	colon2 = NULL;
	string_parse(in, &colon, &colon2, &atsign, &c);

/* if there were two colons, netware has no slash so don't try any
   expansion;   if there was only one colon, make sure the character
   after the colon is a slash and if not, insert one;   if there was
   an at sign, just copy the string */

	if (atsign) {
		strcpy(out, in);
#ifdef STACK_EFFICIENT
		gds__free((SLONG *) local);
#endif /* STACK_EFFICIENT */
		return file_length;
	}
	else if (colon) {
			for (p = in; *p; p++) {
				*out++ = *p;
				if (p == colon && !colon2 && p[1] != '/' && p[1] != '\\' && c)
					*out++ = c;
			}
	}
	else {
		/* assuming the filename is not already remote, look for
		   an environment variable that will provide a default
		   remote path specification */

		if (default_directory = getenv(ISC_DATABASE)) {

			/* once again, do some parsing, this time of the default path */

			string_parse(default_directory, &colon, &colon2, &atsign, &c);

			/* move the default path, putting in a trailing slash, if
			   necessary, and putting in a slash at the start of the
			   file name after the node name, if necessary */

			for (p = default_directory; *p; p++) {
				*out++ = *p;
				if (p == colon && !colon2 && p[1] != '/' && p[1] != '\\' && c)
					*out++ = c;
			}
			if (out[-1] != '/' && out[-1] != '\\' && c)
				*out++ = c;
		}

		for (p = in; *p;)
			*out++ = *p++;
	}

	*out = '\0';

/* If there is an explicit node name of the form \\DOPEY or //DOPEY
   assume named pipes and translate forward slashes to back slashes.
   Otherwise, translate back slashes to forward. The end result
   is a path with consistent separators. */

	if ((expanded_name[0] == '\\' && expanded_name[1] == '\\') ||
		(expanded_name[0] == '/' && expanded_name[1] == '/')) {
		convert_slashes('/', '\\', expanded_name);
	}
	else {
		convert_slashes('\\', '/', expanded_name);
	}


#ifdef STACK_EFFICIENT
	gds__free((SLONG *) local);
#endif /* STACK_EFFICIENT */

	return (int) (out - expanded_name);
}
#endif /* PC_PLATFORM */


#ifdef NETWARE_386
int ISC_expand_filename(
						TEXT * file_name,
						USHORT file_length, TEXT * expanded_name)
{
/**************************************
 *
 *	I S C _ e x p a n d _ f i l e n a m e	( N E T W A R E _ 3 8 6 )
 *
 **************************************
 *
 * Functional description
 *	Just copy a file name for the DOS Requester
 *
 **************************************/
#ifdef STACK_REDUCTION
	TEXT *path;
	TEXT *volume;
	TEXT *directory;
	TEXT *directory1;
	TEXT *fname;
	TEXT *ext;
#else /* STACK_REDUCTION */
	TEXT path[BUFFER_SMALL];
	TEXT volume[BUFFER_SMALL];
	TEXT directory[BUFFER_SMALL];
	TEXT directory1[BUFFER_SMALL];
	TEXT fname[BUFFER_SMALL];
	TEXT ext[BUFFER_SMALL];
#endif /* STACK_REDUCTION */
	TEXT *p;

#ifdef STACK_REDUCTION
	path = (TEXT *) gds__alloc((SLONG) (sizeof(TEXT) * 6 * BUFFER_SMALL));
	volume = path + BUFFER_SMALL;
	directory = volume + BUFFER_SMALL;
	directory1 = directory + BUFFER_SMALL;
	fname = directory1 + BUFFER_SMALL;
	ext = fname + BUFFER_SMALL;
#endif /* STACK_REDUCTION */

	if (!file_length)
		file_length = strlen(file_name);

	strncpy(path, file_name, file_length);
	path[file_length] = 0;

/* use the NetWare routine splitpath to break down the
   path into discrete parts */

	_splitpath(path, volume, directory1, fname, ext);

/* if no volume was specified, use SYS: as the default */

	if (!volume[0])
		strcpy(volume, "SYS:");

/* if no backslash was used at the beginning,
   add it for consistency */

	if ((directory1[0] != '/') && (directory1[0] != '\\')) {
		directory[0] = '\\';
		strcpy(directory + 1, directory1);
	}
	else
		strcpy(directory, directory1);

	strcpy(expanded_name, volume);
	strcat(expanded_name, directory);
	strcat(expanded_name, fname);
	strcat(expanded_name, ext);

	for (p = expanded_name; *p; p++)
		if (*p == '/')
			*p = '\\';
		else
			*p = UPPER7(*p);

#ifdef STACK_REDUCTION
	gds__free((SLONG *) path);
#endif /* STACK_REDUCTION */

	return strlen(expanded_name);
}
#endif


#ifdef VMS
int ISC_expand_filename(
						TEXT * file_name,
						USHORT file_length, TEXT * expanded_name)
{
/**************************************
 *
 *	I S C _ e x p a n d _ f i l e n a m e		( V M S )
 *
 **************************************
 *
 * Functional description
 *	Fully expand a file name.  If the file doesn't exist, do something
 *	intelligent.
 *
 **************************************/
	int l, length, status;
	TEXT *address, temp[NAM$C_MAXRSS], temp2[NAM$C_MAXRSS], *p;
	struct FAB fab;
	struct NAM nam;

	length = ISC_expand_logical(file_name, file_length, expanded_name);

	for (p = expanded_name; *p; p++)
		if (p[0] == ':' && p[1] == ':')
			return length;

	fab = cc$rms_fab;
	nam = cc$rms_nam;
	fab.fab$l_nam = &nam;
	nam.nam$l_esa = temp;
	nam.nam$b_ess = sizeof(temp);
	nam.nam$l_rsa = temp2;
	nam.nam$b_rss = sizeof(temp2);
	fab.fab$l_fna = expanded_name;
	fab.fab$b_fns = length;
	fab.fab$l_dna = DEFAULT_FILE_NAME;
	fab.fab$b_dns = sizeof(DEFAULT_FILE_NAME) - 1;

	if ((sys$parse(&fab) & 1) && (sys$search(&fab) & 1)) {
		p = temp2;
		if (length = l = nam.nam$b_rsl)
			do
				*expanded_name++ = *p++;
			while (--l);
		*expanded_name = 0;
	}

	return length;
}
#endif


#ifdef WIN_NT
int ISC_expand_filename(
						TEXT * file_name,
						USHORT file_length, TEXT * expanded_name)
{
/**************************************
 *
 *	I S C _ e x p a n d _ f i l e n a m e		( W I N _ N T )
 *
 **************************************
 *
 * Functional description
 *	Fully expand a file name.  If the file doesn't exist, do something
 *	intelligent.
 *
 **************************************/
	TEXT *p, *q, *end, temp[MAXPATHLEN], device[4];
	USHORT length = 0;
	USHORT dtype;
	BOOLEAN fully_qualified_path = FALSE;
	BOOLEAN drive_letter_present = FALSE;
#ifdef SPX
	TEXT c;
	TEXT *colon, *colon2, *atsign;
#endif

	if (!file_length)
		file_length = strlen(file_name);

	strncpy(temp, file_name, file_length);
	temp[file_length] = 0;

	expand_share_name(temp);

/* If there is an explicit node name of the form \\DOPEY or //DOPEY
   assume named pipes.  Translate forward slashes to back slashes
   and return with no further processing. */

	if ((file_name[0] == '\\' && file_name[1] == '\\') ||
		(file_name[0] == '/' && file_name[1] == '/')) {
		strcpy(expanded_name, temp);

		/* Translate forward slashes to back slashes */
		for (p = temp, end = p + file_length; p < end; p++)
			if (*p == '/')
				*p = '\\';
		return file_length;
	}

#ifdef SPX
/* If the filename contains a SPX node name, don't even try to expand it.
   But first make sure that the node name is not a drive letter. */
	c = 0;
	colon2 = NULL;
	string_parse(file_name, &colon, &colon2, &atsign, &c);

	if (atsign || colon2) {
		strcpy(expanded_name, temp);
		return (file_length);
	}
/* If the filename contains a TCP node name, don't even try to expand it.
   But first make sure that the node name is not a drive letter. */

	if (q = colon)
#else
	if (q = strchr(temp, INET_FLAG))
#endif
	{
		strcpy(expanded_name, temp);
		if (q - temp != 1)
			return file_length;
		device[0] = temp[0];
		drive_letter_present = TRUE;
		strcpy(device + 1, ":\\");
		dtype = GetDriveType(device);
		if (dtype <= 1)
			return file_length;
		if (*(q + 1) == '/' || *(q + 1) == '\\')
			fully_qualified_path = TRUE;
	}

/* Translate forward slashes to back slashes */

	for (p = temp, end = p + file_length; p < end; p++)
		if (*p == '/')
			*p = '\\';

/* If there is an explicit node name of the form \\DOPEY don't do any
   additional translations -- everything will need to be applied at
   the other end */

	if (temp[0] == '\\' && temp[1] == '\\') {
		strcpy(expanded_name, temp);
		return file_length;
	}
	else if (temp[0] == '\\' || temp[0] == '/')
		fully_qualified_path = TRUE;

/* Expand the file name */

#ifdef SUPERSERVER
	if (!fully_qualified_path)
		length = JRD_getdir(expanded_name, MAXPATHLEN);
	if (length && length < MAXPATHLEN) {
		/**
	case where temp is of the form "c:foo.gdb" and
	expanded_name is "c:\x\y".
        **/
		if (drive_letter_present && device[0] == expanded_name[0]) {
			strcat(expanded_name, "\\");
			strcat(expanded_name, temp + 2);
		}
		/**
	case where temp is of the form "foo.gdb" and
	expanded_name is "c:\x\y".
        **/
		else if (!drive_letter_present) {
			strcat(expanded_name, "\\");
			strcat(expanded_name, temp);
		}
		else {
		/**
	case where temp is of the form "d:foo.gdb" and
	expanded_name is "c:\x\y".
	Discard expanded_name and use temp as it is.
	**/
			/* in this case use the temp but we need to ensure that we expand to
			 * temp from "d:foo.gdb" to "d:\foo.gdb" */
			if (_fullpath(expanded_name, temp, MAXPATHLEN) != NULL) {
				TEXT expanded_name2[MAXPATHLEN];

				/* convert then name to its shorter version ie. convert
				 * longfilename.gdb to longfi~1.gdb */
				length =
					(USHORT) GetShortPathName(expanded_name, expanded_name2,
											  MAXPATHLEN);
				if (length && length < MAXPATHLEN)
					strcpy(expanded_name, expanded_name2);
			}
			else
				strcpy(expanded_name, temp);
		}
		file_length = strlen(expanded_name);
	}
#else
	length = (USHORT) GetFullPathName(temp, MAXPATHLEN, expanded_name, &p);
	if (length && length < MAXPATHLEN)
		file_length = length;
#endif
	else {
		if (_fullpath(expanded_name, temp, MAXPATHLEN) != NULL) {
			TEXT expanded_name2[MAXPATHLEN];

			/* convert then name to its shorter version ie. convert longfilename.gdb
			 * to longfi~1.gdb */
			file_length =
				(USHORT) GetShortPathName(expanded_name, expanded_name2,
										  MAXPATHLEN);
			if (file_length && file_length < MAXPATHLEN)
				strcpy(expanded_name, expanded_name2);
			else
	            file_length = (USHORT) strlen(expanded_name);
		}
		else
			file_length = (USHORT) strlen(expanded_name);
			/* CVC: I know this is incorrect. If _fullpath (that in turn calls GetFullPathName)
					returns NULL, the path + file given are invalid, but the original and useless code
					set length=0 that has no effect and setting file_length to zero stops the code below
					from uppercasing the filename. Following the logic in the prior block of code, the
					action to take is to get the length of the output buffer. Unfortunately, there's
					no function that checks the result of ISC_expand_filename. Since _fullpath is
					GetFullPathName with some checks, the code above looks strange when SUPERSERVER
					is not defined. I decided to make file_length as the length of the output buffer. */
	}

/* Filenames are case insensitive on NT.  If filenames are
 * typed in mixed cases, strcmp () used in various places
 * results in incorrect behavior.
 */

	for (length = 0; length < file_length; length++)
		expanded_name[length] = UPPER7(expanded_name[length]);

	return file_length;
}
#endif


#ifdef VMS
int ISC_expand_logical(
					   TEXT * file_name,
					   USHORT file_length, TEXT * expanded_name)
{
/**************************************
 *
 *	I S C _ e x p a n d _ l o g i c a l
 *
 **************************************
 *
 * Functional description
 *	Fully expand a file name.  If the file doesn't exist, do something
 *	intelligent.
 *
 **************************************/
	int status, attr;
	USHORT n, l;
	TEXT *p;
	ITM items[2];
	struct dsc$descriptor_s desc1, desc2;

	if (!file_length)
		file_length = strlen(file_name);

	ISC_make_desc(file_name, &desc1, file_length);
	ISC_make_desc(LOGICAL_NAME_TABLE, &desc2, sizeof(LOGICAL_NAME_TABLE) - 1);

	items[0].itm_length = 256;
	items[0].itm_code = LNM$_STRING;
	items[0].itm_buffer = expanded_name;
	items[0].itm_return_length = &l;

	items[1].itm_length = 0;
	items[1].itm_code = 0;

	attr = LNM$M_CASE_BLIND;

	if (l = file_length) {
		p = expanded_name;
		do
			*p++ = *file_name++;
		while (--l);
	}

	for (n = 0; n < 10; n++) {
		status = sys$trnlnm(&attr, &desc2, &desc1, NULL, items);
		if (!(status & 1))
			break;
		desc1.dsc$a_pointer = expanded_name;
		desc1.dsc$w_length = file_length = l;
	}

	expanded_name[file_length] = 0;

	return file_length;
}
#endif


#if defined(WIN_NT)
int ISC_expand_share(TEXT * file_name, TEXT * expanded_name)
{
/**************************************
 *
 *	I S C _ e x p a n d _ s h a r e
 *
 **************************************
 *
 * Functional description
 *	Expand a file name by chasing shared disk
 *	information.
 *
 **************************************/
	TEXT *p, device[4];
	USHORT dtype;
	USHORT i;
	HANDLE handle;
	LPNETRESOURCE resources = NULL, res;
/* see NT reference for WNetEnumResource for the following constants */
	DWORD nument = 0xffffffff, size = 16384, ret;
	LPREMOTE_NAME_INFO res2 = NULL;

	strcpy(expanded_name, file_name);

/* Look for a drive letter and make sure that it corresponds
   to a remote disk. */

	if (!(p = strchr(file_name, ':')) || p - file_name != 1)
		return strlen(expanded_name);

	device[0] = toupper(*file_name);
	strcpy(device + 1, ":\\");

	dtype = GetDriveType(device);
	if (dtype != DRIVE_REMOTE)
		return strlen(expanded_name);
	if (WNetOpenEnum(RESOURCE_CONNECTED, RESOURCETYPE_DISK, 0, NULL, &handle)
		!= NO_ERROR)
		return strlen(expanded_name);
	resources = (LPNETRESOURCE) gds__alloc((SLONG) size);
/* FREE: in this routine */
	if (!resources)				/* NOMEM: don't expand the filename */
		return strlen(expanded_name);
	ret = WNetEnumResource(handle, &nument, resources, &size);
	if (ret == ERROR_MORE_DATA) {
		gds__free((UCHAR *) resources);
		resources = (LPNETRESOURCE) gds__alloc((SLONG) size);
		/* FREE: in this routine */
		if (!resources)			/* NOMEM: don't expand the filename */
			return strlen(expanded_name);
		ret = WNetEnumResource(handle, &nument, resources, &size);
	}

	for (i = 0, res = resources; i < nument && *device != *(res->lpLocalName);
		 i++, res++);
	if (i != nument)			/* i.e. we found the drive in the resources list */
		share_name_from_resource(expanded_name, file_name, res);

	(void) WNetCloseEnum(handle);

/* Win95 doesn't seem to return shared drives, so the following
   has been added... */

	if (i == nument) {
		device[2] = 0;
		res2 = (LPREMOTE_NAME_INFO) resources;
		ret =
			WNetGetUniversalName(device, REMOTE_NAME_INFO_LEVEL, res2, &size);
		if (ret == ERROR_MORE_DATA) {
			gds__free((UCHAR *) resources);
			resources = (LPNETRESOURCE) gds__alloc((SLONG) size);
			if (!resources)		/* NOMEM: don't expand the filename */
				return strlen(expanded_name);
			res2 = (LPREMOTE_NAME_INFO) resources;
			ret =
				WNetGetUniversalName(device, REMOTE_NAME_INFO_LEVEL, res2,
									 &size);
		}
		if (ret == NO_ERROR)
			share_name_from_unc(expanded_name, file_name, res2);
	}


	if (resources)
		gds__free((UCHAR *) resources);
	return strlen(expanded_name);
}
#endif	// WIN_NT


#ifdef PARSE_FILENAME
#ifndef REQUESTER
void ISC_parse_filename(
						TEXT * filename,
						TEXT * node, TEXT * device, TEXT * path)
{
/**************************************
 *
 *	I S C _ p a r s e _ f i l e n a m e
 *
 **************************************
 *
 * Functional description
 *	Decompose a PC filename into node name, device name,
 *	and path name.  Leading slashes are retained for node names,
 *	as are trailing colons on device names.
 *
 **************************************/
	TEXT c, *p, slash;

	slash = (*filename == '/') ? '/' : '\\';

	if (filename[0] == slash && filename[1] == slash) {
		*node++ = *filename++;
		*node++ = *filename++;
		while (*filename && *filename != slash)
			*node++ = *filename++;
	}

	*node = 0;

	for (p = filename; *p && *p != ':'; p++);

	if (*p) {
		if (*filename == slash)
			++filename;
		while (filename < p)
			*device++ = *filename++;
		*device++ = *filename++;
	}

	*device = 0;

	while (c = *filename++)
		*path++ = (c == '/') ? '\\' : c;

	*path = 0;
}
#endif
#endif


#ifdef SUPERSERVER
int ISC_strip_extension(TEXT * file_name)
{
/**************************************
 *
 *      I S C _ s t r i p _ e x t e n s i o n 	( S U P E R S E R V E R )
 *
 **************************************
 *
 * Functional description
 *	Get rid of the file name extension part
 *	(after the dot '.')
 *
 **************************************/
	TEXT *p, *q;

/* Set p to point to the starting part of the actual file name
   (sans directory name) */

	p = strrchr(file_name, '/');
	q = strrchr(file_name, '\\');

	if (p || q) {
		/* Get the maximum of the two */

		if (q > p)
			p = q;
	}
	else
		p = file_name;

/* Now search for the first dot in the actual file name */

	q = strchr(p, '.');
	if (q)
		*q = '\0';				/* Truncate the extension including the dot */

	return strlen(file_name);
}
#endif


#if (defined NFS || defined FREEBSD || defined NETBSD || defined SINIXZ)
static int expand_filename2(TEXT * from_buff, USHORT length, TEXT * to_buff)
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
#ifdef STACK_EFFICIENT
	TEXT *temp, *temp2;
#else /* STACK_EFFICIENT */
	TEXT temp[MAXPATHLEN], temp2[MAXPATHLEN];
#endif /* STACK_EFFICIENT */
	TEXT *from, *to, *p, *segment;
	SSHORT n;
	struct passwd *passwd;

#ifdef STACK_EFFICIENT
	temp = (TEXT *) gds__alloc((SLONG) (sizeof(TEXT) * 2 * MAXPATHLEN));
	temp2 = temp + MAXPATHLEN;
#endif /* STACK_EFFICIENT */

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
#ifdef STACK_EFFICIENT
		gds__free((SLONG *) temp);
#endif /* STACK_EFFICIENT */
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
			*to = 0;
		}
	}

/* If the file is local, expand partial pathnames with default directory */

	if (*from && !strchr(from, INET_FLAG) && *from != '/' && GETWD(to)) {
		while (*to)
			++to;
		*to++ = '/';
		*to = 0;
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

		n = readlink(to_buff, temp, MAXPATHLEN);

		if (n < 0)
			continue;

		/* We've got a link.  If it contains a node name or it starts
		   with a slash, it replaces the initial segment so far */

		temp[n] = 0;
		p = temp;

		if (strchr(temp, INET_FLAG)) {
			strcpy(to_buff, temp);
#ifdef STACK_EFFICIENT
			gds__free((SLONG *) temp);
#endif /* STACK_EFFICIENT */
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

#ifdef STACK_EFFICIENT
	gds__free((SLONG *) temp);
#endif /* STACK_EFFICIENT */

	return to - to_buff;
}
#endif


#ifdef WIN_NT
static void expand_share_name(TEXT * share_name)
{
/**************************************
 *
 *	e x p a n d _ s h a r e _ n a m e
 *
 **************************************
 *
 * Functional description
 *	Look for a Windows NT share name at the
 *	beginning of a string having the form:
 *
 *	    \!share name!\file name
 *
 *	If such a share name is found, expand it
 *	and then append the file name to the
 *	expanded path.
 *
 **************************************/

#ifdef STACK_EFFICIENT
	TEXT *data_buf, *workspace;
#else /* STACK_EFFICIENT */
	TEXT data_buf[MAXPATHLEN], workspace[MAXPATHLEN];
#endif /* STACK_EFFICIENT */
	TEXT *p, *q, *data;
	HKEY hkey;
	DWORD ret, d_size, type_code;
	USHORT idx;

	p = share_name;
	if (*p++ != '\\' || *p++ != '!') {
		return;
	}

#ifdef STACK_EFFICIENT
	data_buf = (TEXT *) gds__alloc((SLONG) (sizeof(TEXT) * 2 * MAXPATHLEN));
	workspace = data_buf + MAXPATHLEN;
#endif /* STACK_EFFICIENT */
	strcpy(workspace, p);
	for (q = workspace; *p && *p != '!'; p++, q++);

	*q = '\0';
	if (*p++ != '!' || *p++ != '\\') {
#ifdef STACK_EFFICIENT
		gds__free((SLONG *) data_buf);
#endif /* STACK_EFFICIENT */
		return;
	}

	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
					 "SYSTEM\\CurrentControlSet\\Services\\LanmanServer\\Shares",
					 0, KEY_QUERY_VALUE, &hkey) != ERROR_SUCCESS) {
#ifdef STACK_EFFICIENT
		gds__free((SLONG *) data_buf);
#endif /* STACK_EFFICIENT */
		return;
	}

	d_size = MAXPATHLEN;
	data = data_buf;
#pragma FB_COMPILER_MESSAGE("Can/should we fix this cast?")
	ret =
		RegQueryValueEx(hkey, workspace, NULL, &type_code, (LPBYTE) data,
						&d_size);
	if (ret == ERROR_MORE_DATA) {
		d_size++;
		data = (TEXT *) gds__alloc((SLONG) d_size);
		/* FREE: unknown */
		if (!data) {			/* NOMEM: */
#ifdef STACK_EFFICIENT
			gds__free((SLONG *) data_buf);
#endif /* STACK_EFFICIENT */
			RegCloseKey(hkey);
			return;				/* Error not really handled */
		}
		ret =
			RegQueryValueEx(hkey, workspace, NULL, &type_code, (LPBYTE) data,
							&d_size);
	}

	if (ret == ERROR_SUCCESS) {
		for (q = data; q && *q;
			 q = (type_code == REG_MULTI_SZ) ? q + strlen(q) + 1 : NULL) {
			if (!strnicmp(q, "path", 4)) {
		    /* CVC: Paranoid protection against buffer overrun.
				    MAXPATHLEN minus NULL terminator, the possible backslash and p==db_name.
					Otherwise, it's possible to create long share plus long db_name => crash. */
				idx = strlen(q + 5);
				if (idx + 1 + (q[4 + idx] == '\\' ? 1 : 0) + strlen(p) >= MAXPATHLEN)
					break;

				strcpy(workspace, q + 5);	/* step past the "Path=" part */
			    /* idx = strlen (workspace); Done previously. */
				if (workspace[idx - 1] != '\\')
					workspace[idx++] = '\\';
				strcpy(workspace + idx, p);
				strcpy(share_name, workspace);
				break;
			}
		}
	}

	if (data != data_buf) {
		gds__free(data);
	}

#ifdef STACK_EFFICIENT
	gds__free((SLONG *) data_buf);
#endif /* STACK_EFFICIENT */

	RegCloseKey(hkey);
}
#endif


#ifdef NFS
#ifdef ultrix
#define GET_MOUNTS
static BOOLEAN get_mounts(MNT * mount, TEXT * buffer, int *context)
{
/**************************************
 *
 *	g e t _ m o u n t s	( U l t r i x )
 *
 **************************************
 *
 * Functional description
 *	Get ALL mount points.
 *
 **************************************/
	TEXT *p, *q;
	struct fs_data fs;

/* Start by finding a mount point. */

	p = buffer;

	while (getmnt(context, &fs, sizeof(fs), NOSTAT_MANY, NULL) > 0) {
		/* Include non-NFS (local) mounts - some may be longer than
		   NFS mount points */

/****
    if (fs.fd_req.fstype != GT_NFS)
	continue;
****/

		mount->mnt_node = p;
		q = fs.fd_req.devname;
		while (*q && *q != ':')
			*p++ = *q++;
		*p++ = 0;
		if (*q != ':')
			mount->mnt_node = NULL;
		if (*q)
			q++;
		mount->mnt_path = p;
		while (*p++ = *q++);
		mount->mnt_mount = p;
		q = fs.fd_req.path;
		while (*p++ = *q++);
		return TRUE;
	}

	return FALSE;
}
#endif


#if (defined AIX || defined AIX_PPC)
#define GET_MOUNTS
static BOOLEAN get_mounts(
						  MNT * mount,
						  TEXT * mnt_buffer, TEXT ** buffer, int *count)
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
		if (!(*buffer = gds__alloc((SLONG) l)) ||
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


#ifdef DECOSF
#define GET_MOUNTS
static BOOLEAN get_mounts(
						  MNT * mount,
						  TEXT * mnt_buffer, TEXT ** buffer, int *count)
{
/**************************************
 *
 *	g e t _ m o u n t s	( D E C O S F )
 *
 **************************************
 *
 * Functional description
 *	Get ALL mount points.
 *
 **************************************/
	long bufsize;
	TEXT *q;
	struct statfs *fs;

	if (!*buffer) {
		/* The first time through, get the mount info from the system.
		   First find out how much information there is, then allocate
		   a buffer for it, and finally get it. */

		bufsize = 0;
		*count = getfsstat(0, bufsize, MNT_NOWAIT);
		if (*count <= 0)
			return FALSE;

		bufsize = sizeof(struct statfs) * (SLONG) * count;
		/* FREE: in get_mounts() */
		if (!(*buffer = gds__alloc((SLONG) bufsize)) ||
			(*count = getfsstat(*buffer, bufsize, MNT_NOWAIT)) <= 0)
			return FALSE;		/* NOMEM: */
	}

	if (!(*count)--)
		return FALSE;

/* Include non-NFS (local) mounts - some may be longer than
   NFS mount points */

	fs = &((struct statfs *) *buffer)[*count];
	mount->mnt_node = mnt_buffer;
	q = fs->f_mntfromname;
	while (*q && *q != ':')
		*mnt_buffer++ = *q++;
	*mnt_buffer++ = 0;
	if (*q != ':')
		mount->mnt_node = NULL;
	if (*q)
		q++;
	mount->mnt_path = mnt_buffer;
	while (*mnt_buffer++ = *q++);
	mount->mnt_mount = fs->f_mntonname;

	return TRUE;
}
#endif


#if defined(SV_MNTENT) && !defined(SOLARIS)
#define GET_MOUNTS
static BOOLEAN get_mounts(MNT * mount, TEXT * buffer, IB_FILE * file)
{
/**************************************
 *
 *	g e t _ m o u n t s	( S Y S T E M _ V )
 *				( E P S O N )
 *				( M 8 8 K )
 *				( U N I X W A R E )
 *
 **************************************
 *
 * Functional description
 *	Get ALL mount points.
 *
 **************************************/
	TEXT *p, *q;
	struct mnttab *mptr, mnttab;

/* Start by finding a mount point. */

	p = buffer;

	mptr = &mnttab;
	if (getmntent(file, mptr) == 0) {
		/* Include non-NFS (local) mounts - some may be longer than
		   NFS mount points */

		mount->mnt_node = p;
		q = mptr->mnt_special;
		while (*q && *q != ':')
			*p++ = *q++;
		*p++ = 0;
		if (*q != ':')
			mount->mnt_node = NULL;
		if (*q)
			q++;
		mount->mnt_path = p;
	    while ((*p++ = *q++) != 0);
		mount->mnt_mount = mptr->mnt_mountp;
		return TRUE;
	}
	else
		return FALSE;
}
#endif
#ifdef SOLARIS
#define GET_MOUNTS
static BOOLEAN get_mounts(MNT * mount, TEXT * buffer, IB_FILE * file)
{
/**************************************
 *
 *	g e t _ m o u n t s	( S O L A R I S )
 *
 **************************************
 *
 * Functional description
 *	Get ALL mount points.
 *
 **************************************/

	/* Since we had to substitute our alternative for the stdio supplied
	   with Solaris, we cannot use the getmntent() library call which
	   wants a Solaris stdio FILE* as an argument, so we parse the text-
	   type /etc/mnttab file ourselves. */

	TEXT device[128], mount_point[128], type[128], rw[128], foo1[128], *p, *q;
	SSHORT n;

/* Start by finding a mount point. */

	p = buffer;

	for (;;) {
		/* device, mountpoint, type, options, a number */

		n = ib_fscanf(file, "%s %s %s %s %s",
					  device, mount_point, type, rw, foo1);

		/* This should either be 5 (we got a line with 5 args, or EOF (we
		   got nothing. If we're short of 5, assume EOF */
		if (n != 5)
			break;

		/* Include non-NFS (local) mounts - some may be longer than
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
		if (*q != ':')
			mount->mnt_node = NULL;
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


#ifdef SCO_UNIX
#define GET_MOUNTS
static BOOLEAN get_mounts(MNT * mount, TEXT * buffer, IB_FILE * file)
{
/**************************************
 *
 *	g e t _ m o u n t s	( S C O - U N I X )
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

/* note that the mount point and device are inverted from normal systems */
	for (;;) {
		/* Sake of argument, inverted the mount_point, device */

		n =
			ib_fscanf(file, "%s %s %s %s %s %s %s %s %s %s", mount_point,
					  foo1, device, rw, foo1, foo1, foo1, foo1, foo1, foo1);
		if (!strcmp(rw, "read"))
			n = ib_fscanf(file, "%s", foo1);

		if (n < 0)
			break;

		/* Include non-NFS (local) mounts - some may be longer than
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
		if (*q != ':')
			mount->mnt_node = NULL;
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


#ifndef NON_MNTENT
#define GET_MOUNTS
static BOOLEAN get_mounts(MNT * mount, TEXT * buffer, IB_FILE * file)
{
/**************************************
 *
 *	g e t _ m o u n t s	( M N T E N T )
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

	while (mptr = getmntent(file)) {
		/* Include non-NFS (local) mounts - some may be longer than
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
		if (*q != ':')
			mount->mnt_node = NULL;
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


#ifndef GET_MOUNTS
static BOOLEAN get_mounts(MNT * mount, TEXT * buffer, IB_FILE * file)
{
/**************************************
 *
 *	g e t _ m o u n t s	( g e n e r i c - U N I X )
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

		/* Include non-NFS (local) mounts - some may be longer than
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
		if (*q != ':')
			mount->mnt_node = NULL;
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


#ifdef hpux
/* RITTER - added HP11 to the pre-processor condition below */
#if !(defined HP10 || defined HP11)
static BOOLEAN get_server(TEXT * file_name, TEXT * node_name)
{
/**************************************
 *
 *	g e t _ s e r v e r		( H P - U X )
 *
 **************************************
 *
 * Functional description
 *	If we're running on a cnode, the file system beSLONGs
 *	to the server node - load node_name with the server
 *	name and return TRUE.
 *
 **************************************/
	TEXT *p, hostname[64];
	struct cct_entry *cnode;

	cnode = getccnam(ISC_get_host(hostname, sizeof(hostname)));
	if (!cnode || cnode->cnode_type == 'r')
		return FALSE;

	setccent();
	while (cnode->cnode_type != 'r')
		cnode = getccent();
	strncpy(node_name, cnode->cnode_name, sizeof(cnode->cnode_name));

	return TRUE;
}
#endif
#endif
#endif


#ifdef WIN_NT
static void share_name_from_resource(
									 TEXT * expanded_name,
									 TEXT * filename, LPNETRESOURCE resource)
{
/**************************************
 *
 *	s h a r e _ n a m e _ f r o m _ r e s o u r c e
 *
 **************************************
 *
 * Functional description
 *	if the shared drive is Windows or Novell prosess the
 *	name appropriately, otherwise just return the remote name
 *	expects filename to be of the form DRIVE_LETTER:\PATH
 *	returns new filename in expanded_name sholdn't touch filename
 *
 **************************************/
	TEXT *p = resource->lpRemoteName, *q = expanded_name, *r = filename;

/* If the shared drive is via Windows
   package it up so that resolution of the share name can
   occur on the remote machine. The name
   that will be transmitted to the remote machine will
   have the form \\REMOTE_NODE\!SHARE_POINT!\FILENAME */

	if (!strnicmp(resource->lpProvider, "Microsoft Windows Network", 26)) {
		*q++ = *p++;
		*q++ = *p++;
		while ((*q++ = *p++) != '\\');
		*q++ = '!';
		while (*q++ = *p++);
		q--;					/* after the above q points to the char AFTER the null */
		*q++ = '!';
		*q = '\0';
		strcat(expanded_name, filename + 2);
	}
/* from \\NODE\VOLUME & filename to NODE@VOLUME:filename */
	else if (!strnicmp(resource->lpProvider, "NetWare", 7)) {
		p += 2;					/* skip past \\ */
		while (*p != '\\')
			*q++ = *p++;
		*p++;
		*q++ = '@';
		while (*p && *p != '\\')
			*q++ = *p++;
		*q++ = ':';
		*q = '\0';
		if (*p)
			strcat(expanded_name, p);
		strcat(expanded_name, filename + 2);
	}
	else {						/* we're guessing that it might be an NFS shared drive */

		while (*q++ = *p++);
		q -= 2;					/* after the above q points to the char AFTER the null */
		if (*q == '\\' || *q == '//')	/* chop off any trailing \ or / */
			*q = '\0';
		strcat(expanded_name, filename + 2);

		/* If the expanded filename doesn't begin with a node name of the form
		   \\NODE and it contains a ':', then it's probably an NSF mounted drive.
		   Therefore we must convert any back slashes to forward slashes. */

		if ((*expanded_name != '\\' || *(expanded_name + 1) != '\\')
			&& (q = strchr(expanded_name, INET_FLAG))) {
			while (*q) {
				if (*q == '\\')
					*q = '/';
				q++;
			}
		}
	}
}


static void share_name_from_unc(
								TEXT * expanded_name,
								TEXT * file_name,
								LPREMOTE_NAME_INFO unc_remote)
{
/**************************************
 *
 *      s h a r e _ n a m e _ f r o m _ u n c
 *
 **************************************
 *
 * Functional description
 *      Extract the share name from a REMOTE_NAME_INFO struct
 *      returned by WNetGetUniversalName.   It uses only the
 *      lpConnectionName element of the structure.   It converts
 *      "\\node\sharepoint" to "\\node\!sharepoint!" and appends
 *      the rest of file_name after the drive into expanded_name.
 *
 **************************************/
	TEXT *p, *q;


	p = unc_remote->lpConnectionName;
	q = expanded_name;

	/* copy the \\ and the node name */

	*q++ = *p++;
	*q++ = *p++;
	while ((*q++ = *p++) != '\\');

	/* bracket the share name with "!" characters */

	*q++ = '!';
	while (*q++ = *p++);
	q--;						/* after the above, q points to the char AFTER the null */
	*q++ = '!';
	*q = '\0';

	/* add rest of file name */

	strcat(expanded_name, file_name + 2);
}
#endif /* WIN_NT */


#if (defined SPX) || ((defined PC_PLATFORM) && !(defined NETWARE_386))
static void string_parse(
						 TEXT * string,
						 TEXT ** colon,
						 TEXT ** colon2, TEXT ** atsign, TEXT * slashchar)
{
/**************************************
 *
 *      s t r i n g _ p a r s e
 *
 **************************************
 *
 * Functional description
 *      Get the first and second colon and the first at sign
 *      positions from a string, plus do some verifications.
 *      Also return whether there's a slash or backslash
 *      in the string.
 *
 **************************************/
	TEXT *p;

/* if there were both an at sign and a colon, and if the at sign follows
   the colon, the at sign should be considered part of the file name and
   not part of an spx node name */

	*colon = strchr(string, ':');
	*atsign = strchr(string, '@');
	if (*atsign && *colon && (*atsign > *colon))
		*atsign = NULL;

/* if there was a colon and no at sign, look for a second colon, which
   implies a netware node */

	if (*colon && !(*atsign))
		*colon2 = strchr(*colon + 1, ':');
	for (p = string; *p && !(*slashchar); p++)
		if (*p == '/' || *p == '\\')
			*slashchar = *p;
}

static void convert_slashes(char oldslash, char newslash, char *string)
{
	char *p;

	for (p = string; *p != 0; p++) {
		if (*p == oldslash)
			*p = newslash;
	}
}

#endif /* PC_PLATFORM */

