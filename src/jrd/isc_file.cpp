/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		isc_file.cpp
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

#include "firebird.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../jrd/common.h"
#include "gen/iberror.h"
#include "../jrd/jrd.h"
#include "../jrd/gds_proto.h"
#include "../jrd/isc_proto.h"
#include "../jrd/isc_f_proto.h"
#include "../jrd/jrd_proto.h"
#include "../common/config/config.h"
#include "../common/config/dir_list.h"
#include "../common/classes/init.h"

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


const char* LOGICAL_NAME_TABLE	= "LNM$FILE_DEV";
const char* DEFAULT_FILE_NAME	= ".fdb";
const char INET_FLAG		= '^';

struct itm {
	SSHORT itm_length;
	SSHORT itm_code;
	SCHAR *itm_buffer;
	SSHORT *itm_return_length;
};
typedef itm ITM;

#else /* of ifdef VMS */
const char INET_FLAG		= ':';
#endif


#ifdef SUPERSERVER
#define GETWD(buf)		JRD_getdir(buf)
#else
#define GETWD(buf)		fb_getcwd(buf)
#endif /* SUPERSERVER */


/* Unix/NFS specific stuff */
#ifndef NO_NFS
#ifdef HAVE_MNTENT_H
#include <mntent.h>	/* get setmntent/endmntent */
#endif
#ifdef HAVE_SYS_MNTTAB_H
#include <sys/mnttab.h>	/* get MNTTAB/_PATH_MNTTAB */
#endif

#ifdef MNTTAB
const char* MTAB		= MNTTAB;
#elif defined(_PATH_MNTTAB)
const char* MTAB		= _PATH_MNTTAB;
#elif defined(hpux)
const char* MTAB		= "/etc/mnttab";
#elif defined(SOLARIS)
const char* MTAB		= "/etc/mnttab";
#elif defined(FREEBSD)
const char* MTAB		= "/etc/fstab";
#elif defined(SCO_UNIX)
const char* MTAB		= "/etc/mount";
#else
const char* MTAB		= "/etc/mtab";
#endif

#ifdef HAVE_SETMNTENT
#define MTAB_OPEN(path,type)	setmntent(path, "r")
#define MTAB_CLOSE(stream)	endmntent(stream)
#elif defined(SCO_UNIX)
/* EKU: popen/pclose to access a file??? */
#define MTAB_OPEN(path,type)	popen (path, type)
#define MTAB_CLOSE(stream)	pclose (stream)
#else
#define MTAB_OPEN(path,type)	fopen(path, type)
#define MTAB_CLOSE(stream)	fclose(stream)
#endif
#endif /* NO_NFS */


#ifdef hpux
/* RITTER - added HP11 to the pre-processor condition below */
#if !(defined HP10 || defined HP11)
#include <cluster.h>
#endif
#endif

#if (defined AIX || defined AIX_PPC)
#include <sys/vmount.h>
#endif


struct mnt {
	TEXT *mnt_node;
	TEXT *mnt_mount;
	TEXT *mnt_path;
};
typedef mnt MNT;


#ifndef MAXHOSTLEN
#define MAXHOSTLEN	64
#endif

#if (!defined NO_NFS || defined FREEBSD || defined NETBSD || defined SINIXZ)
static int expand_filename2(const Firebird::PathName&, Firebird::PathName&);
#endif

#if defined(WIN_NT)
static void translate_slashes(Firebird::PathName&);
static void expand_share_name(Firebird::PathName&);
static void share_name_from_resource(TEXT*, USHORT, const TEXT*, LPNETRESOURCE);
static void share_name_from_unc(TEXT*, USHORT, const TEXT*, LPREMOTE_NAME_INFO);
static bool get_full_path(const Firebird::PathName&, Firebird::PathName&);
#endif

#ifndef NO_NFS
#if (defined AIX || defined AIX_PPC)
static bool get_mounts(MNT *, TEXT *, TEXT **, int *);
#else
static bool get_mounts(MNT *, TEXT *, FILE *);
#endif
#endif

#ifdef hpux
/* RITTER - added HP11 to the pre-processor condition below */
#if !(defined HP10 || defined HP11)
static bool get_server(TEXT*, TEXT*);
#endif
#endif


#ifndef NO_NFS
bool ISC_analyze_nfs(TEXT* expanded_filename, TEXT* node_name)
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

	TEXT mnt_buffer[BUFFER_LARGE],
		max_node[MAXHOSTLEN], max_path[MAXPATHLEN], expand_mount[MAXPATHLEN];

    // If we are ignoring NFS remote mounts then do not bother checking here
    // and pretend it's only local. MOD 16-Nov-2002
    if (! Config::getRemoteFileOpenAbility()) {
        return false;
    }

	int len = 0;
	*max_path = 0;
	bool flag = false;

/* Search mount points */

	TEXT* temp = NULL;
	int context = 0;

	MNT mount;
#if (defined AIX || defined AIX_PPC)
	while (get_mounts(&mount, mnt_buffer, &temp, &context))
#else
	FILE* mtab = MTAB_OPEN(MTAB, "r");
	if (!mtab) {
		return flag;
	}
	while (get_mounts(&mount, mnt_buffer, mtab))
#endif
	{
		/* first, expand any symbolic links in the mount point */

		ISC_expand_filename(mount.mnt_mount, 0, expand_mount, sizeof(expand_mount));
		mount.mnt_mount = expand_mount;

		/* see how much of the mount point matches the expanded_filename */

		const TEXT* p = expanded_filename;
		const TEXT* q = mount.mnt_mount;
		while (*q && *q == *p++)
			++q;

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

	TEXT remote_filename[MAXPATHLEN];

	if (*max_path) {
		TEXT* p = remote_filename;

		const TEXT* q = max_path;
		while (*q)
			*p++ = *q++;

		q = expanded_filename + len;
		while (*p++ = *q++);

		q = max_node;
		while (*node_name++ = *q++);

		p = remote_filename;
		while (*expanded_filename++ = *p++);

		flag = true;
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
int ISC_analyze_pclan(TEXT* expanded_name, TEXT* node_name)
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
	TEXT localhost[64];

	if ((expanded_name[0] != '\\' && expanded_name[0] != '/') ||
		(expanded_name[1] != '\\' && expanded_name[1] != '/'))
		return FALSE;

	TEXT* p = node_name;
	*p++ = '\\';
	*p++ = '\\';
	const TEXT* q;
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


int ISC_analyze_tcp(TEXT* file_name, TEXT* node_name)
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

/* Scan file name looking for separator character */

	TEXT* p = strchr(file_name, INET_FLAG);
	if (!p)
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


bool ISC_check_if_remote(const Firebird::PathName& file_name,
							bool implicit_flag)
{
/**************************************
 *
 *	I S C _ c h e c k _ i f _ r e m o t e
 *
 **************************************
 *
 * Functional description
 *	Check to see if a path name resolves to a
 *	remote file.  If implicit_flag is true, then
 *	analyze the path to see if it resolves to a
 *	file on a remote machine.  Otherwise, simply
 *	check for an explicit node name.
 *
 **************************************/
	TEXT temp_name[MAXPATHLEN];

	strncpy(temp_name, file_name.c_str(), MAXPATHLEN);
	temp_name[MAXPATHLEN - 1] = 0;

/* Always check for an explicit TCP node name */

	TEXT host_name[64];
	if (ISC_analyze_tcp(temp_name, host_name)) {

		return true;
	}
#ifndef NO_NFS
	if (implicit_flag) {
		/* Check for a file on an NFS mounted device */

		if (ISC_analyze_nfs(temp_name, host_name)) {
			return true;
		}
	}
#endif

#if defined(WIN_NT)
/* Check for an explicit named pipe node name */

	if (ISC_analyze_pclan(temp_name, host_name)) {

		return true;
	}

	if (implicit_flag) {
		TEXT temp_name2[MAXPATHLEN];

		/* Check for a file on a shared drive.  First try to expand
		   the path.  Then check the expanded path for a TCP or
		   named pipe. */

		ISC_expand_share(temp_name, temp_name2, sizeof(temp_name2));
		if (ISC_analyze_tcp(temp_name2, host_name) ||
			ISC_analyze_pclan(temp_name2, host_name))
		{
			return true;
		}

	}
#endif	// WIN_NT

	return false;
}


#if (!defined NO_NFS || defined FREEBSD || defined NETBSD || defined SINIXZ)
int ISC_expand_filename(const Firebird::PathName& from_buff, Firebird::PathName& to_buff)
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

	return expand_filename2(from_buff, to_buff);
}
#endif


#ifdef VMS
int ISC_expand_filename(const TEXT* file_name,
						USHORT file_length, TEXT* expanded_name, USHORT bufsize)
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
	TEXT temp[NAM$C_MAXRSS], temp2[NAM$C_MAXRSS];

	int length = ISC_expand_logical(file_name, file_length, expanded_name, bufsize);

	TEXT* p;
	for (p = expanded_name; *p; p++)
		if (p[0] == ':' && p[1] == ':')
			return length;

	struct FAB fab = cc$rms_fab;
	struct NAM nam = cc$rms_nam;
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
		int l = length = nam.nam$b_rsl
		if (l)
			do {
				if (bufsize-- == 1)
					break;
				*expanded_name++ = *p++;
			} while (--l);
		*expanded_name = 0;
	}

	return length;
}
#endif


#ifdef WIN_NT

static void translate_slashes(Firebird::PathName& Path)
{
    const char sep = '\\';
    const char bad_sep = '/';
	for (char *p = Path.begin(), *q = Path.end(); p < q; p++)
	{
		if (*p == bad_sep) {
			*p = sep;
		}
	}
}

// Code of this function is a slightly changed version of this routine
// from Jim Barry (jim.barry@bigfoot.com) published at 
// http://www.geocities.com/SiliconValley/2060/articles/longpaths.html

static bool ShortToLongPathName(
	const Firebird::PathName& ShortPath,
    Firebird::PathName& LongPath)
{
	// Special characters.
    const char sep = '\\';
    const char colon = ':';
    // Make some short type aliases
    typedef Firebird::PathName tstring;
    typedef tstring::size_type size;
    const size npos = tstring::npos;

    // Copy the short path into the work buffer and convert forward 
    // slashes to backslashes.
	LongPath = ShortPath;
	translate_slashes(LongPath);

    // We need a couple of markers for stepping through the path.
    size left = 0;
    size right = 0;

    // Parse the first bit of the path.
    if (LongPath.length() >= 2 && isalpha(LongPath[0]) && colon == LongPath[1]) // Drive letter?
    {
        if (2 == LongPath.length()) // 'bare' drive letter
        {
            right = npos; // skip main block
        }
        else if (sep == LongPath[2]) // drive letter + backslash
        {
            // FindFirstFile doesn't like "X:\"
            if (3 == LongPath.length())
            {
                right = npos; // skip main block
            }
            else
            {
                left = right = 3;
            }
        }
        else
		{
			return false; // parsing failure
		}
    }
    else if (LongPath.length() >= 1 && sep == LongPath[0])
    {
        if (1 == LongPath.length()) // 'bare' backslash
        {
            right = npos;  // skip main block
        }
        else 
        {
            if (sep == LongPath[1]) // is it UNC?
            {
                // Find end of machine name
                right = LongPath.find_first_of(sep, 2);
                if (npos == right) 
				{
                    return false;
				}

                // Find end of share name
                right = LongPath.find_first_of(sep, right + 1);
                if (npos == right)
				{
                    return false;
				}
            }
            ++right;
        }
    }
    // else FindFirstFile will handle relative paths

    // The data block for FindFirstFile.
    WIN32_FIND_DATA fd;

    // Main parse block - step through path.
	HANDLE hf = INVALID_HANDLE_VALUE;
    while (npos != right)
    {
        left = right; // catch up

        // Find next separator.
        right = LongPath.find_first_of(sep, right);

        // Temporarily replace the separator with a null character so that
        // the path so far can be passed to FindFirstFile.
        if (npos != right)
            LongPath[right] = 0;

        // See what FindFirstFile makes of the path so far.
        hf = FindFirstFile(LongPath.c_str(), &fd);
        if (INVALID_HANDLE_VALUE == hf)
			break;
        FindClose(hf);

        // Put back the separator.
        if (npos != right)
            LongPath[right] = sep;

        // The file was found - replace the short name with the long.
        const size old_len = (npos == right) ? LongPath.length() - left : right - left;
        const size new_len = strlen(fd.cFileName);
        LongPath.replace(left, old_len, fd.cFileName, new_len);

        // More to do?
        if (npos != right)
        {
            // Yes - move past separator .
            right = left + new_len + 1;

            // Did we overshoot the end? (i.e. path ends with a separator).
            if (right >= LongPath.length())
                right = npos;
        }
    }

	// We failed to find this file.
    if (INVALID_HANDLE_VALUE == hf)
        return false;

    return true;
}

int ISC_expand_filename(const Firebird::PathName& file_name,
						Firebird::PathName& expanded_name)
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
    typedef Firebird::PathName tstring;
    typedef tstring::size_type size;
    const size npos = tstring::npos;

	// check for empty filename to avoid multiple checks later
	if (!file_name.length()) {
		expanded_name = file_name;
		return 0;
	}

	bool fully_qualified_path = false;
	tstring temp = file_name;

	expand_share_name(temp);

/* If there is an explicit node name of the form \\DOPEY or //DOPEY
   assume named pipes.  Translate forward slashes to back slashes
   and return with no further processing. */

	if ((file_name.length() >= 2) && 
		((file_name[0] == '\\' && file_name[1] == '\\') ||
		 (file_name[0] == '/' && file_name[1] == '/')))
	{
		expanded_name = temp;

		/* Translate forward slashes to back slashes */
		translate_slashes(expanded_name);
		return expanded_name.length();
	}

	size colon_pos;
	tstring device;
	if ((colon_pos = temp.find(INET_FLAG)) != npos)
	{
		expanded_name = temp;
		if (colon_pos != 1)
			return expanded_name.length();
		device = temp[0];
		device += ":\\";
		const USHORT dtype = GetDriveType(device.c_str());
		if (dtype <= 1)
			return expanded_name.length();
		
		if ((temp.length() >= 3) && (temp[2] == '/' || temp[2] == '\\')) {
			fully_qualified_path = true;
		}
	}

/* Translate forward slashes to back slashes */

	translate_slashes(temp);

/* If there is an explicit node name of the form \\DOPEY don't do any
   additional translations -- everything will need to be applied at
   the other end */

	if ((temp.length() >= 2) && (temp[0] == '\\' && temp[1] == '\\')) {
		expanded_name = temp;
		return expanded_name.length();
	}
	if (temp[0] == '\\' || temp[0] == '/') {
		fully_qualified_path = true;
	}
/* Expand the file name */

#ifdef SUPERSERVER
	if ((!fully_qualified_path) && JRD_getdir(expanded_name)) {
		/**
	case where temp is of the form "c:foo.fdb" and
	expanded_name is "c:\x\y".
        **/
		if (device.hasData() && device[0] == expanded_name[0]) {
			expanded_name += '\\';
			expanded_name.append (temp, 2, npos);
		}
		/**
	case where temp is of the form "foo.fdb" and
	expanded_name is "c:\x\y".
        **/
		else if (device.empty()) {
			expanded_name += '\\';
			expanded_name += temp;
		}
		else {
		/**
	case where temp is of the form "d:foo.fdb" and
	expanded_name is "c:\x\y".
	Discard expanded_name and use temp as it is.
	**/
			/* in this case use the temp but we need to ensure that we expand to
			 * temp from "d:foo.fdb" to "d:\foo.fdb" */
			if (!get_full_path(temp, expanded_name))
				expanded_name = temp;
		}
	}
	else
#endif
	{
		if (!get_full_path(temp, expanded_name))
			expanded_name = temp;
	}

	/* convert then name to its longer version ie. convert longfi~1.fdb
	 * to longfilename.fdb */
	if (ShortToLongPathName(expanded_name, temp)) {
		expanded_name = temp;
	}

	// Filenames are case insensitive on NT.  If filenames are
	// typed in mixed cases, strcmp () used in various places
	// results in incorrect behavior.
	expanded_name.upper();

	return expanded_name.length();
}
#endif


#ifdef VMS
int ISC_expand_logical(const TEXT* file_name,
					   USHORT file_length, TEXT* expanded_name, USHORT bufsize)
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
	ITM items[2];
	struct dsc$descriptor_s desc1, desc2;

	if (!file_length)
		file_length = strlen(file_name);

	ISC_make_desc(file_name, &desc1, file_length);
	ISC_make_desc(LOGICAL_NAME_TABLE, &desc2, sizeof(LOGICAL_NAME_TABLE) - 1);

	USHORT l;
	items[0].itm_length = bufsize; //256;
	items[0].itm_code = LNM$_STRING;
	items[0].itm_buffer = expanded_name;
	items[0].itm_return_length = &l;

	items[1].itm_length = 0;
	items[1].itm_code = 0;

	int attr = LNM$M_CASE_BLIND;

	if (l = file_length) {
		if (l > bufsize)
		    l = bufsize;
		TEXT* p = expanded_name;
		do {
			*p++ = *file_name++;
		} while (--l);
	}

	for (int n = 0; n < 10; n++) {
		const int status = sys$trnlnm(&attr, &desc2, &desc1, NULL, items);
		if (!(status & 1))
			break;
		desc1.dsc$a_pointer = expanded_name;
		desc1.dsc$w_length = file_length = l;
	}

	if (file_length >= bufsize)
		file_length = bufsize - 1;

	expanded_name[file_length] = 0;

	return file_length;
}
#endif


#if defined(WIN_NT)
int ISC_expand_share(const TEXT* file_name, TEXT* expanded_name, USHORT bufsize)
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
/* see NT reference for WNetEnumResource for the following constants */
	DWORD nument = 0xffffffff, size = 16384;

	strncpy(expanded_name, file_name, bufsize);
	expanded_name[bufsize - 1] = 0;

/* Look for a drive letter and make sure that it corresponds
   to a remote disk. */
   const int exp_name_len = strlen(expanded_name);

	const TEXT* p = strchr(file_name, ':');
	if (!p || p - file_name != 1)
		return exp_name_len;

	TEXT device[4];
	device[0] = toupper(*file_name);
	strcpy(device + 1, ":\\");

	const USHORT dtype = GetDriveType(device);
	if (dtype != DRIVE_REMOTE)
		return exp_name_len;
		
	HANDLE handle;
	if (WNetOpenEnum(RESOURCE_CONNECTED, RESOURCETYPE_DISK, 0, NULL, &handle)
		!= NO_ERROR)
	{
		return exp_name_len;
	}
	LPNETRESOURCE resources = (LPNETRESOURCE) gds__alloc((SLONG) size);
/* FREE: in this routine */
	if (!resources)				/* NOMEM: don't expand the filename */
		return exp_name_len;
		
	DWORD ret = WNetEnumResource(handle, &nument, resources, &size);
	if (ret == ERROR_MORE_DATA) {
		gds__free(resources);
		resources = (LPNETRESOURCE) gds__alloc((SLONG) size);
		/* FREE: in this routine */
		if (!resources)			/* NOMEM: don't expand the filename */
			return exp_name_len;
			
		ret = WNetEnumResource(handle, &nument, resources, &size);
	}

	LPNETRESOURCE res = resources;
	DWORD i = 0;
	while (i < nument && (!res->lpLocalName || (*device != *(res->lpLocalName)))) {
		i++;
		res++;
	}
	if (i != nument)			/* i.e. we found the drive in the resources list */
		share_name_from_resource(expanded_name, bufsize, file_name, res);

	WNetCloseEnum(handle);

/* Win95 doesn't seem to return shared drives, so the following
   has been added... */

	if (i == nument) {
		device[2] = 0;
		LPREMOTE_NAME_INFO res2 = (LPREMOTE_NAME_INFO) resources;
		ret =
			WNetGetUniversalName(device, REMOTE_NAME_INFO_LEVEL, res2, &size);
		if (ret == ERROR_MORE_DATA) {
			gds__free(resources);
			resources = (LPNETRESOURCE) gds__alloc((SLONG) size);
			// share_name_from_resource wasn't invoked, so exp_name_len still applies.
			if (!resources)		/* NOMEM: don't expand the filename */
				return exp_name_len;
				
			res2 = (LPREMOTE_NAME_INFO) resources;
			ret =
				WNetGetUniversalName(device, REMOTE_NAME_INFO_LEVEL, res2,
									 &size);
		}
		if (ret == NO_ERROR)
			share_name_from_unc(expanded_name, bufsize, file_name, res2);
	}


	if (resources)
		gds__free(resources);
		
	// cannot use exp_name_len here because share_name_from_(resource|unc)
	// may have altered it.
	return strlen(expanded_name);
}
#endif	// WIN_NT

#ifdef SUPERSERVER
int ISC_strip_extension(TEXT* file_name)
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

/* Set p to point to the starting part of the actual file name
   (sans directory name) */

	TEXT* p = strrchr(file_name, '/');
	TEXT* q = strrchr(file_name, '\\');

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
static int expand_filename2(const Firebird::PathName& from_buff, Firebird::PathName& to_buff)
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
	typedef Firebird::PathName string;
	typedef string::size_type size;
	const size npos = string::npos;
	string src = from_buff;	

// If the filename contains a TCP node name, don't even try to expand it
	if (src.find(INET_FLAG) != npos) {
		to_buff = src;
		return to_buff.length();
	}

	const char* from = src.c_str();
	to_buff = "";

// Handle references to default directories (tilde refs)
	if (*from == '~') {
		++from;
		string q;
		while (*from && *from != '/')
			q += *from++;
		const struct passwd* password =
			q.hasData() ? getpwnam(q.c_str()) : getpwuid(geteuid());
		if (password) {
			expand_filename2(password->pw_dir, to_buff);
		}
	}

// If the file is local, expand partial pathnames with default directory
	if (*from && *from != '/') {
		if (! GETWD(to_buff)) {
			to_buff = "";
		}
		to_buff += '/';
	}

// Process file name segment by segment looking for symbolic links.
// See ISC_analyze_nfs for how NFS mount points are  handled.
	while (*from) {

		// skip dual // (will collapse /// to / as well)
		if (*from == '/' && from[1] == '/') {
			++from;
			continue;
		}

		// Copy the leading slash, if any
		if (*from == '/') {
			if (to_buff.hasData() && (to_buff.end()[-1] == '/'))
				++from;
			else
				to_buff += *from++;
			continue;
		}

		// Handle self references
		if (*from == '.' && (from[1] == '.' || from[1] == '/')) {
			if (*++from == '.') {
				++from;
				if (to_buff.length() > 2) {
					const size slash = to_buff.rfind('/', to_buff.length() - 2);
					to_buff = slash != npos ? to_buff.substr(0, slash + 1) : "/";
				}
			}
			continue;
		}

		// Copy the rest of the segment name
		const int segment = to_buff.length();
		while (*from && *from != '/') {
			to_buff += *from++;
		}

		// If the file is local, check for a symbol link
		TEXT temp[MAXPATHLEN];
		const int n = readlink(to_buff.c_str(), temp, MAXPATHLEN);
		if (n < 0)
			continue;

		// We've got a link.  If it contains a node name or it starts
		// with a slash, it replaces the initial segment so far.
		string link(temp, n);
		if (link.find(INET_FLAG) != npos) {
			to_buff = link;
			return to_buff.length();
		}
		if (link[0] == '/') {
			to_buff = link;
		}
		else {
			to_buff.replace(segment, to_buff.length() - segment, link);
		}

		/* Whole link needs translating -- recurse */
		expand_filename2(to_buff, to_buff);
	}

	return to_buff.length();
}
#endif


#ifdef WIN_NT
static void expand_share_name(Firebird::PathName& share_name)
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

	TEXT workspace[MAXPATHLEN];

	const TEXT* p = share_name.c_str();
	if (*p++ != '\\' || *p++ != '!') {
		return;
	}

	strncpy(workspace, p, sizeof(workspace));
	workspace[sizeof(workspace) - 1] = 0;
	// We test for *q, too, to avoid buffer overrun.
	TEXT* q;
	for (q = workspace; *q && *p && *p != '!'; p++, q++);
	// empty body loop
	*q = '\0';
	if (*p++ != '!' || *p++ != '\\') {
		return;
	}

	HKEY hkey;
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
					 "SYSTEM\\CurrentControlSet\\Services\\LanmanServer\\Shares",
					 0, KEY_QUERY_VALUE, &hkey) != ERROR_SUCCESS) 
	{
		return;
	}

	BYTE data_buf[MAXPATHLEN];
	DWORD d_size = MAXPATHLEN;
	DWORD type_code;
	LPBYTE data = data_buf;

	DWORD ret =
		RegQueryValueEx(hkey, workspace, NULL, &type_code, data, &d_size);
	if (ret == ERROR_MORE_DATA) {
		d_size++;
		data = (LPBYTE) gds__alloc((SLONG) d_size);
		/* FREE: unknown */
		if (!data) {			/* NOMEM: */
			RegCloseKey(hkey);
			return;				/* Error not really handled */
		}
		ret =
			RegQueryValueEx(hkey, workspace, NULL, &type_code, data, &d_size);
	}

	if (ret == ERROR_SUCCESS) {
		for (const TEXT* s = reinterpret_cast<const TEXT*>(data); s && *s;
			 s = (type_code == REG_MULTI_SZ) ? s + strlen(s) + 1 : NULL)
		{
			if (!strnicmp(s, "path", 4)) {
		    /* CVC: Paranoid protection against buffer overrun.
				    MAXPATHLEN minus NULL terminator, the possible backslash and p==db_name.
					Otherwise, it's possible to create long share plus long db_name => crash. */
				size_t idx = strlen(s + 5);
				if (idx + 1 + (s[4 + idx] == '\\' ? 1 : 0) + strlen(p) >= MAXPATHLEN)
					break;

				strcpy(workspace, s + 5);	/* step past the "Path=" part */
			    /* idx = strlen (workspace); Done previously. */
				if (workspace[idx - 1] != '\\')
					workspace[idx++] = '\\';
				strcpy(workspace + idx, p);
				share_name = workspace;
				break;
			}
		}
	}

	if (data != data_buf) {
		gds__free(data);
	}

	RegCloseKey(hkey);
}

static bool get_full_path(const Firebird::PathName& part, 
						  Firebird::PathName& full)
{
	TEXT buf[MAXPATHLEN];
	TEXT *p;
	const int l = GetFullPathName(part.c_str(), MAXPATHLEN, buf, &p);
	if (l && l < MAXPATHLEN) {
		full = buf;
		return true;
	}
	return false;
}
#endif


#ifndef NO_NFS
#if (defined AIX || defined AIX_PPC)
#define GET_MOUNTS
static bool get_mounts(
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
	if (!*buffer) {
		/* The first time through, get the mount info from the system.
		   First find out how much information there is, then allocate
		   a buffer for it, and finally get it. */

		if (mntctl(MCTL_QUERY, sizeof(SLONG), mnt_buffer) != 0)
			return false;

		const int l = *(SLONG *) mnt_buffer;
		/* FREE: in get_mounts() */
		if (!(*buffer = gds__alloc((SLONG) l)) ||
			(*count = mntctl(MCTL_QUERY, l, *buffer)) <= 0)
		{
			return false;		/* NOMEM: */
		}
	}
	else if (!*count)
		return false;

	TEXT* p;
	for (int i = --(*count), p = *buffer; i--;)
		p += ((struct vmount *) p)->vmt_length;

	struct vmount* vmt = (struct vmount *) p;

	mount->mnt_node = mnt_buffer;
	p = vmt2dataptr(vmt, VMT_HOSTNAME);
	int l = vmt2datasize(vmt, VMT_HOSTNAME);
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

	return true;
}
#endif // (defined AIX || defined AIX_PPC)

#if defined(HAVE_GETMNTENT) && !defined(SOLARIS)
#define GET_MOUNTS
#if defined(GETMNTENT_TAKES_TWO_ARGUMENTS) /* SYSV stylish */
static bool get_mounts(MNT * mount, TEXT * buffer, FILE * file)
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
	struct mnttab *mptr, mnttab;

/* Start by finding a mount point. */

	TEXT* p = buffer;

	mptr = &mnttab;
	if (getmntent(file, mptr) == 0) {
		/* Include non-NFS (local) mounts - some may be longer than
		   NFS mount points */

		mount->mnt_node = p;
		const TEXT* q = mptr->mnt_special;
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
		return true;
	}
	else
		return false;
}
#else // !GETMNTENT_TAKES_TWO_ARGUMENTS 
static bool get_mounts(MNT * mount, TEXT * buffer, FILE * file)
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
	struct mntent *mptr;

/* Start by finding a mount point. */

	TEXT* p = buffer;

	while ((mptr = getmntent(file)) != (struct mntent *)0) {
		/* Include non-NFS (local) mounts - some may be longer than
		   NFS mount points */

/****
    if (strcmp (mptr->mnt_type, MNTTYPE_NFS))
	continue;
****/

		mount->mnt_node = p;
		const TEXT* q = mptr->mnt_fsname;
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
		return true;
	}

	return false;
}
#endif // GETMNTENT_TAKES_TWO_ARGUMENTS
#endif // HAVE_GETMNTENT && !SOLARIS

#ifdef SCO_UNIX
#define GET_MOUNTS
static bool get_mounts(MNT * mount, TEXT * buffer, FILE * file)
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
	TEXT device[128], mount_point[128], type[16], rw[128], foo1[16];

/* Start by finding a mount point. */

	TEXT* p = buffer;

/* note that the mount point and device are inverted from normal systems */
	for (;;) {
		/* Sake of argument, inverted the mount_point, device */

		int n =
			fscanf(file, "%s %s %s %s %s %s %s %s %s %s", mount_point,
					  foo1, device, rw, foo1, foo1, foo1, foo1, foo1, foo1);
		if (!strcmp(rw, "read"))
			n = fscanf(file, "%s", foo1);

		if (n < 0)
			break;

		/* Include non-NFS (local) mounts - some may be longer than
		   NFS mount points */

/****
    if (strcmp (type, "nfs"))
	continue;
****/

		mount->mnt_node = p;
		const TEXT* q = device;
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
		return true;
	}

	return false;
}
#endif // SCO_UNIX

#ifndef GET_MOUNTS
static bool get_mounts(MNT * mount, TEXT * buffer, FILE * file)
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
	
	TEXT device[128], mount_point[128], type[16], rw[128], foo1[16];

/* Start by finding a mount point. */

	TEXT* p = buffer;

	for (;;) {
		const int n =
			fscanf(file, "%s %s %s %s %s %s", device, mount_point, type, rw, foo1, foo1);
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
		const TEXT* q = device;
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
		return true;
	}

	return false;
}
#endif // GET_MOUNTS

#ifdef hpux
/* RITTER - added HP11 to the pre-processor condition below */
#if !(defined HP10 || defined HP11)
static bool get_server(TEXT* file_name, TEXT* node_name)
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
	TEXT hostname[64];
	const struct cct_entry* cnode = getccnam(ISC_get_host(hostname, sizeof(hostname)));
	if (!cnode || cnode->cnode_type == 'r')
		return false;

	setccent();
	while (cnode->cnode_type != 'r')
		cnode = getccent();
	strncpy(node_name, cnode->cnode_name, sizeof(cnode->cnode_name));

	return true;
}
#endif
#endif // hpux
#endif // NO_NFS


#ifdef WIN_NT
static void share_name_from_resource(TEXT* expanded_name,
									 USHORT bufsize,
									 const TEXT* filename,
									 LPNETRESOURCE resource)
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
 *	returns new filename in expanded_name; shouldn't touch filename
 *
 **************************************/
	const TEXT* const p = resource->lpRemoteName;
	//TEXT* q = expanded_name;
	Firebird::PathName s;

/* If the shared drive is via Windows
   package it up so that resolution of the share name can
   occur on the remote machine. The name
   that will be transmitted to the remote machine will
   have the form \\REMOTE_NODE\!SHARE_POINT!\FILENAME */

	if (!strnicmp(resource->lpProvider, "Microsoft Windows Network", 26)) {
		const char* p2 = p + 2;
		while (*p2++ != '\\'); // empty loop's body
		s.append(p, p2 - p);
		s += '!';
		s.append(p2);
		s += '!';
		// add rest of file name
		s.append(filename + 2);
		s.copy_to(expanded_name, bufsize);
	}
	else {						/* we're guessing that it might be an NFS shared drive */
		s.append(p);
		char* q = s.end();
		q -= 1;					/* after the above q points to the the null char */
		if (*q == '\\' || *q == '/')	/* chop off any trailing \ or / */
			*q = '\0';
		s.recalculate_length();
		s.append(filename + 2);
		s.copy_to(expanded_name, bufsize);

		/* If the expanded filename doesn't begin with a node name of the form
		   \\NODE and it contains a ':', then it's probably an NSF mounted drive.
		   Therefore we must convert any back slashes to forward slashes. */

		if ((expanded_name[0] != '\\' || expanded_name[1] != '\\')
			&& (q = strchr(expanded_name, INET_FLAG)))
		{
			while (*q) {
				if (*q == '\\')
					*q = '/';
				q++;
			}
		}
	}
}


static void share_name_from_unc(TEXT* expanded_name,
								USHORT bufsize,
								const TEXT* file_name,
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
	const TEXT* const p = unc_remote->lpConnectionName;
	TEXT* q = expanded_name;

	Firebird::PathName s;
	/* copy the \\ and the node name */

	const char* p2 = p + 2;
	while (*p2++ != '\\'); // empty loop's body
	s.append(p, p2 - p);

	/* bracket the share name with "!" characters */

	s += '!';
	s.append(p2);
	s += '!';
	// add rest of file name
	s.append(file_name + 2);
	s.copy_to(expanded_name, bufsize);
}
#endif /* WIN_NT */


#ifndef SUPERCLIENT
namespace {
	class DatabaseDirectoryList : public Firebird::DirectoryList
	{
	private:
		const Firebird::PathName getConfigString(void) const {
			return Firebird::PathName(Config::getDatabaseAccess());
		}
	public:
		DatabaseDirectoryList(MemoryPool& p) : DirectoryList(p) 
		{ 
			initialize();
		}
	};
	Firebird::InitInstance<DatabaseDirectoryList> iDatabaseDirectoryList;
}
#endif

bool ISC_verify_database_access(const Firebird::PathName& name)
{
/**************************************
 *
 *      I S C _ v e r i f y _ d a t a b a s e _ a c c e s s
 *
 **************************************
 *
 * Functional description
 *      Verify 'name' against DatabaseAccess entry of firebird.conf.
 *
 **************************************/
#ifndef SUPERCLIENT
	if (!iDatabaseDirectoryList().isPathInList(name)) {
		return false;
	}
#endif
	return true;
}

