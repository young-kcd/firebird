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
 *
 * 2002.10.27 Sean Leyne - Code Cleanup, removed obsolete "UNIXWARE" port
 * 2002.10.27 Sean Leyne - Code Cleanup, removed obsolete "Ultrix" port
 *
 * 2002.10.28 Sean Leyne - Completed removal of obsolete "DGUX" port
 * 2002.10.28 Sean Leyne - Code cleanup, removed obsolete "DecOSF" port
 *
 * 2002.10.29 Sean Leyne - Removed support for obsolete IPX/SPX Protocol
 * 2002.10.29 Sean Leyne - Removed obsolete "Netware" port
 *
 * 2002.10.30 Sean Leyne - Removed support for obsolete "PC_PLATFORM" define
 * 2002.10.30 Sean Leyne - Code Cleanup, removed obsolete "SUN3_3" port
 *
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
#include "../common/config/config.h"
#include "../common/config/dir_list.h"

#include <sys/types.h>
#ifdef HAVE_SYS_IPC_H
#include <sys/ipc.h>
#endif
#ifdef HAVE_SYS_FILE_H
#include <sys/file.h>
#endif
#include <errno.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_PWD_H
#include <pwd.h>
#endif

#include "../common/config/config.h"

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
#define DEFAULT_FILE_NAME	".fdb"
#define INET_FLAG		'^'

typedef struct itm {
	SSHORT itm_length;
	SSHORT itm_code;
	SCHAR *itm_buffer;
	SSHORT *itm_return_length;
} ITM;

#endif /* of ifdef VMS */


#ifdef SUPERSERVER
#define GETWD(buf)		JRD_getdir(buf, MAXPATHLEN)
#else
#ifdef HAVE_GETCWD
#define GETWD(buf)		getcwd(buf, MAXPATHLEN)
#else
#define GETWD			getwd
#endif
#endif /* SUPERSERVER */


/* Unix/NFS specific stuff */
#ifndef NO_NFS
#ifdef HAVE_MNTENT_H
#include <mntent.h>	/* get setmntent/endmntent */
#endif
#ifdef HAVE_SYS_MNTTAB_H
#include <sys/mnttab.h>	/* get MNTTAB/_PATH_MNTTAB */
#endif

/* EKU: if you get a compiler warning/error about redefinition of MTAB,
        please remove the define from the platform-specific section below
        and not here! */
#ifdef MNTTAB
#define MTAB			MNTTAB
#else
#ifdef _PATH_MNTTAB
#define MTAB			_PATH_MNTTAB
#else
#define MTAB			"/etc/mtab"
#endif
#endif

#ifdef HAVE_SETMNTENT
#define MTAB_OPEN(path,type)	setmntent(path, "r")
#define MTAB_CLOSE(stream)	endmntent(stream)
#else
#define MTAB_OPEN(path,type)	ib_fopen(path, type)
#define MTAB_CLOSE(stream)	ib_fclose(stream)
#endif
#endif /* NO_NFS */


#ifdef hpux
#define MTAB			"/etc/mnttab"
/* RITTER - added HP11 to the pre-processor condition below */
#if !(defined HP10 || defined HP11)
#include <cluster.h>
#endif
#endif

#ifdef SOLARIS
#define MTAB			"/etc/mnttab"
#endif

#ifdef FREEBSD
#define MTAB			"/etc/fstab"
#endif

#ifdef SCO_UNIX
/* EKU: popen/pclose to access a file??? */
#define MTAB			"/etc/mount"
#define MTAB_OPEN(path,type)	popen (path, type)
#define MTAB_CLOSE(stream)	pclose (stream)
#endif

#if (defined AIX || defined AIX_PPC)
#include <sys/vmount.h>
#endif


typedef struct mnt {
	TEXT *mnt_node;
	TEXT *mnt_mount;
	TEXT *mnt_path;
} MNT;


/* Windows NT stuff */
#ifdef WIN_NT
#include <windows.h>
#ifndef TEXT
#define TEXT		SCHAR
#endif
#endif


#ifndef MAXHOSTLEN
#define MAXHOSTLEN	64
#endif

#ifndef INET_FLAG
#define INET_FLAG	':'
#endif

#if (!defined NO_NFS || defined FREEBSD || defined NETBSD || defined SINIXZ)
static int expand_filename2(TEXT *, USHORT, TEXT *);
#endif

#if defined(WIN_NT)
static void expand_share_name(TEXT *);
static void share_name_from_resource(TEXT *, TEXT *, LPNETRESOURCE);
static void share_name_from_unc(TEXT *, TEXT *, LPREMOTE_NAME_INFO);
#endif

#ifndef NO_NFS
#if (defined AIX || defined AIX_PPC)
static BOOLEAN get_mounts(MNT *, TEXT *, TEXT **, int *);
#else
static BOOLEAN get_mounts(MNT *, TEXT *, IB_FILE *);
#endif
#endif

#ifdef hpux
/* RITTER - added HP11 to the pre-processor condition below */
#if !(defined HP10 || defined HP11)
static BOOLEAN get_server(TEXT *, TEXT *);
#endif
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


#ifndef NO_NFS
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


	TEXT mnt_buffer[BUFFER_LARGE], remote_filename[MAXPATHLEN],
		max_node[MAXHOSTLEN], max_path[MAXPATHLEN], expand_mount[MAXPATHLEN];

	MNT mount;
	TEXT *p, *q, *temp;
	USHORT flag;

	IB_FILE *mtab;
	int context, len;


    // If we are ignoring NFS remote mounts then do not bother checking here
    // and pretend it's only local. MOD 16-Nov-2002
    if (! Config::getRemoteFileOpenAbility()) {
        return FALSE;
    }

	len = 0;
	*max_path = 0;
	flag = FALSE;

/* Search mount points */

	temp = NULL;
	context = 0;

#if (defined AIX || defined AIX_PPC)
	while (get_mounts(&mount, mnt_buffer, &temp, &context))
#else
	if (!(mtab = MTAB_OPEN(MTAB, "r"))) {
		return flag;
	}
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

#if (!defined AIX && !defined AIX_PPC)
	MTAB_CLOSE(mtab);
#endif

#ifdef hpux
/* RITTER - added HP11 to the pre-processor condition below */
#if !(defined HP10 || defined HP11)
	if (!flag)
		flag = get_server(expanded_filename, node_name);
#endif
#endif

#if (defined AIX || defined AIX_PPC)
	if (temp)
		gds__free(temp);
#endif

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

/* Scan file name looking for separator character */

	if (!(p = strchr(file_name, INET_FLAG)))
		return FALSE;

#ifdef WIN_NT
/* For Windows NT, insure that a single character node name does
   not conflict with an existing drive letter. */

	if (p - file_name == 1) {
		USHORT dtype;
		*node_name = *file_name;
		strcpy(node_name + 1, ":\\");
		dtype = GetDriveType(node_name);
		if (dtype > 1 && dtype != DRIVE_REMOTE)
			return FALSE;
	}
#endif

	*p = 0;
	strcpy(node_name, file_name);
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
	TEXT temp_name[MAXPATHLEN];
	TEXT host_name[64];

	strcpy(temp_name, file_name);

/* Always check for an explicit TCP node name */

	if (ISC_analyze_tcp(temp_name, host_name)) {

		return TRUE;
	}
#ifndef NO_NFS
	if (implicit_flag) {
		/* Check for a file on an NFS mounted device */

		if (ISC_analyze_nfs(temp_name, host_name)) {
			return TRUE;
		}
	}
#endif

#if defined(WIN_NT)
/* Check for an explicit named pipe node name */

	if (ISC_analyze_pclan(temp_name, host_name)) {

		return TRUE;
	}

	if (implicit_flag) {
		TEXT temp_name2[MAXPATHLEN];

		/* Check for a file on a shared drive.  First try to expand
		   the path.  Then check the expanded path for a TCP or
		   named pipe. */

		ISC_expand_share(temp_name, temp_name2);
		if (ISC_analyze_tcp(temp_name2, host_name) ||
			ISC_analyze_pclan(temp_name2, host_name)) {
			return TRUE;
		}

	}
#endif	// WIN_NT

	return FALSE;
}


#if (!defined NO_NFS || defined FREEBSD || defined NETBSD || defined SINIXZ)
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

	if (q = strchr(temp, INET_FLAG))
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
	case where temp is of the form "c:foo.fdb" and
	expanded_name is "c:\x\y".
        **/
		if (drive_letter_present && device[0] == expanded_name[0]) {
			strcat(expanded_name, "\\");
			strcat(expanded_name, temp + 2);
		}
		/**
	case where temp is of the form "foo.fdb" and
	expanded_name is "c:\x\y".
        **/
		else if (!drive_letter_present) {
			strcat(expanded_name, "\\");
			strcat(expanded_name, temp);
		}
		else {
		/**
	case where temp is of the form "d:foo.fdb" and
	expanded_name is "c:\x\y".
	Discard expanded_name and use temp as it is.
	**/
			/* in this case use the temp but we need to ensure that we expand to
			 * temp from "d:foo.fdb" to "d:\foo.fdb" */
			if (_fullpath(expanded_name, temp, MAXPATHLEN) != NULL) {
				TEXT expanded_name2[MAXPATHLEN];

				/* convert then name to its shorter version ie. convert
				 * longfilename.fdb to longfi~1.fdb */
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

			/* convert then name to its shorter version ie. convert longfilename.fdb
			 * to longfi~1.fdb */
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


#if (!defined NO_NFS || defined FREEBSD || defined NETBSD || defined SINIXZ)
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
	TEXT temp[MAXPATHLEN], temp2[MAXPATHLEN];
	TEXT *from, *to, *p, *segment;
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

		/* skip dual // (will collapse /// to / as well) */
		if (*from == '/' && from[1] == '/') {
			++from;
			continue;
		}

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

	TEXT data_buf[MAXPATHLEN], workspace[MAXPATHLEN];
	TEXT *p, *q, *data;
	HKEY hkey;
	DWORD ret, d_size, type_code;
	USHORT idx;

	p = share_name;
	if (*p++ != '\\' || *p++ != '!') {
		return;
	}

	strcpy(workspace, p);
	for (q = workspace; *p && *p != '!'; p++, q++);

	*q = '\0';
	if (*p++ != '!' || *p++ != '\\') {
		return;
	}

	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
					 "SYSTEM\\CurrentControlSet\\Services\\LanmanServer\\Shares",
					 0, KEY_QUERY_VALUE, &hkey) != ERROR_SUCCESS) {
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

	RegCloseKey(hkey);
}
#endif


#ifndef NO_NFS
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
#endif // (defined AIX || defined AIX_PPC)

#if defined(HAVE_GETMNTENT) && !defined(SOLARIS)
#define GET_MOUNTS
#if defined(GETMNTENT_TAKES_TWO_ARGUMENTS) /* SYSV stylish */
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
#else // !GETMNTENT_TAKES_TWO_ARGUMENTS 
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

	while ((mptr = getmntent(file)) != (struct mntent *)0) {
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
#endif // GETMNTENT_TAKES_TWO_ARGUMENTS
#endif // HAVE_GETMNTENT && !SOLARIS

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
#endif // SCO_UNIX

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
	
/* Solaris uses this because:
	Since we had to substitute an alternative for the stdio supplied
	with Solaris, we cannot use the getmntent() library call which
	wants a Solaris stdio FILE* as an argument, so we parse the text-
	type /etc/mnttab file ourselves.     - from FB1
	
	This will still apply with SFIO on FB2.  nmcc Dec2002
*/
	
	TEXT device[128], mount_point[128], type[16], rw[128], foo1[16], *p, *q;
	SSHORT n;

/* Start by finding a mount point. */

	p = buffer;

	for (;;) {
		n = ib_fscanf(file, "%s %s %s %s %s %s", device, mount_point, type, rw, foo1, foo1);
#ifdef SOLARIS
		if (n != 5)
#else
		if (n < 0)
#endif
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
#endif // GET_MOUNTS

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
 *	If we're running on a cnode, the file system belongs
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
#endif // hpux
#endif // NO_NFS


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
	TEXT *p = resource->lpRemoteName, *q = expanded_name;

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
	else {						/* we're guessing that it might be an NFS shared drive */

		while (*q++ = *p++);
		q -= 2;					/* after the above q points to the char AFTER the null */
		if (*q == '\\' || *q == '/')	/* chop off any trailing \ or / */
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

bool ISC_verify_database_access(TEXT *name) {
#ifndef SUPERCLIENT
	static class DatabaseDirectoryList : public DirectoryList {
		const Firebird::string GetConfigString(void) const {
			return Firebird::string(Config::getDatabaseAccess());
		}
	} iDatabaseDirectoryList;

	if (!iDatabaseDirectoryList.IsPathInList(name)) {
		return false;
	}
#endif
	return true;
}
