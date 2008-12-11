/*
 *      PROGRAM:        JRD Access Method
 *      MODULE:         isc.cpp
 *      DESCRIPTION:    General purpose but non-user routines.
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
 * Solaris x86 changes - Konstantin Kuznetsov, Neil McCalden
 * 26-Sept-2001 Paul Beach - External File Directory Config. Parameter
 * 17-Oct-2001 Mike Nordell: CPU affinity
 * 01-Feb-2002 Paul Reeves: Removed hard-coded registry path
 *
 * 2002.10.28 Sean Leyne - Completed removal of obsolete "DGUX" port
 *
 * 2002.10.29 Sean Leyne - Removed obsolete "Netware" port
 *
 * 2002.10.30 Sean Leyne - Removed support for obsolete "PC_PLATFORM" define
 * 2002.10.30 Sean Leyne - Code Cleanup, removed obsolete "SUN3_3" port
 *
 */

#ifdef DARWIN
#define _STLP_CCTYPE
#endif

#include "firebird.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "../jrd/common.h"

#include "gen/iberror.h"
#include "../jrd/ibase.h"
#include "../jrd/jrd.h"
#include "../jrd/scl.h"
#include "../jrd/gds_proto.h"
#include "../jrd/isc_proto.h"
#include "../jrd/jrd_proto.h"

#include "../common/classes/init.h"

#ifdef SOLARIS
#include <sys/utsname.h>
#endif

#ifdef SCO_EV
#include <sys/utsname.h>
#endif

/* Win32 specific stuff */

#ifdef WIN_NT

#include <windows.h>
#include <aclapi.h>
#include <lmcons.h>

class SecurityAttributes
{
public:
	explicit SecurityAttributes(MemoryPool& pool)
	{
		// Ensure that our process has the SYNCHRONIZE privilege granted to everyone
		PSECURITY_DESCRIPTOR pOldSD = NULL;
		PACL pOldACL = NULL;
		GetSecurityInfo(GetCurrentProcess(), SE_KERNEL_OBJECT,
						DACL_SECURITY_INFORMATION,
						NULL, NULL, &pOldACL, NULL, &pOldSD);

		// NULL pOldACL means all privileges. If we assign pNewACL in this case
		// we'll lost all privileges except assigned SYNCHRONIZE
		if (pOldACL)
		{
			SID_IDENTIFIER_AUTHORITY sidAuth = SECURITY_WORLD_SID_AUTHORITY;
			PSID pSID = NULL;
			AllocateAndInitializeSid(&sidAuth, 1, SECURITY_WORLD_RID,
									 0, 0, 0, 0, 0, 0, 0, &pSID);

			EXPLICIT_ACCESS ea;
			memset(&ea, 0, sizeof(EXPLICIT_ACCESS));
			ea.grfAccessPermissions = SYNCHRONIZE;
			ea.grfAccessMode = GRANT_ACCESS;
			ea.grfInheritance = NO_INHERITANCE;
			ea.Trustee.TrusteeForm = TRUSTEE_IS_SID;
			ea.Trustee.TrusteeType = TRUSTEE_IS_WELL_KNOWN_GROUP;
			ea.Trustee.ptstrName  = (LPTSTR) pSID;

			PACL pNewACL = NULL;
			SetEntriesInAcl(1, &ea, pOldACL, &pNewACL);

			SetSecurityInfo(GetCurrentProcess(), SE_KERNEL_OBJECT,
							DACL_SECURITY_INFORMATION,
							NULL, NULL, pNewACL, NULL);

			if (pSID) {
				FreeSid(pSID);
			}
			if (pNewACL) {
				LocalFree(pNewACL);
			}
		}

		if (pOldSD) {
			LocalFree(pOldSD);
		}

		// Create and initialize the default security descriptor
		// to be assigned to various IPC objects.
		//
		// WARNING!!! The absent DACL means full access granted
		// to everyone, this is a huge security risk!

		PSECURITY_DESCRIPTOR p_security_desc = pool.allocate(SECURITY_DESCRIPTOR_MIN_LENGTH);

		attributes.nLength = sizeof(attributes);
		attributes.lpSecurityDescriptor = p_security_desc;
		attributes.bInheritHandle = TRUE;

		if (!InitializeSecurityDescriptor(p_security_desc, SECURITY_DESCRIPTOR_REVISION) ||
			!SetSecurityDescriptorDacl(p_security_desc, TRUE, NULL, FALSE))
		{
			delete p_security_desc;
			attributes.lpSecurityDescriptor = NULL;
		}
	}

	~SecurityAttributes()
	{
		delete attributes.lpSecurityDescriptor;
	}

	operator LPSECURITY_ATTRIBUTES()
	{
		return attributes.lpSecurityDescriptor ? &attributes : NULL;
	}

private:
	SECURITY_ATTRIBUTES attributes;
};

static Firebird::InitInstance<SecurityAttributes> security_attributes;

#endif // WIN_NT


#ifdef HAVE_SIGNAL_H
#include <signal.h>
#endif

#ifdef HAVE_CTYPE_H
#include <ctype.h>
#endif

/* Unix specific stuff */

#if defined(UNIX)
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#ifdef HAVE_PWD_H
#include <pwd.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#endif

#ifndef O_RDWR
#include <fcntl.h>
#endif


bool ISC_check_process_existence(SLONG pid)
{
/**************************************
 *
 *      I S C _ c h e c k _ p r o c e s s _ e x i s t e n c e
 *
 **************************************
 *
 * Functional description
 *      Return TRUE if the indicated process
 *      exists.  FALSE otherwise.
 *
 **************************************/

#ifdef WIN_NT
	const HANDLE handle = OpenProcess(SYNCHRONIZE, FALSE, (DWORD) pid);

	if (!handle)
	{
		return (GetLastError() == ERROR_ACCESS_DENIED);
	}

	const bool alive = (WaitForSingleObject(handle, 0) != WAIT_OBJECT_0);
	CloseHandle(handle);
	return alive;
#else
	return (kill((int) pid, 0) == -1 &&	errno == ESRCH) ? false : true;
#endif
}


#if (defined SOLARIS || defined SCO_EV)
TEXT* ISC_get_host(TEXT* string, USHORT length)
{
/**************************************
 *
 *      I S C _ g e t _ h o s t                 ( S O L A R I S )
 *
 **************************************
 *
 * Functional description
 *      Get host name.
 *
 **************************************/
	struct utsname name;

	if (uname(&name) >= 0)
	{
		strncpy(string, name.nodename, length);
		string[length - 1] = 0;
	}
	else
		strcpy(string, "local");

	return string;
}

#elif defined(WIN_NT)

TEXT* ISC_get_host(TEXT* string, USHORT length)
{
/**************************************
 *
 *      I S C _ g e t _ h o s t                 ( W I N _ N T )
 *
 **************************************
 *
 * Functional description
 *      Get host name.
 * Note that this is not the DNS (TCP/IP) hostname,
 * it's the Win32 computer name.
 *
 **************************************/
	DWORD host_len = length;
	if (GetComputerName(string, &host_len))
	{
		string[host_len] = 0;
	}
	else
	{
		strcpy(string, "local");
	}

	return string;
}

#else

TEXT* ISC_get_host(TEXT* string, USHORT length)
{
/**************************************
 *
 *      I S C _ g e t _ h o s t                 ( G E N E R I C )
 *
 **************************************
 *
 * Functional description
 *      Get host name.
 *
 **************************************/
	// See http://www.opengroup.org/onlinepubs/007908799/xns/gethostname.html
	if (gethostname(string, length))
		string[0] = 0; // failure
	else
		string[length - 1] = 0; // truncation doesn't guarantee null termination

	return string;
}
#endif

const TEXT* ISC_get_host(Firebird::string& host)
{
/**************************************
 *
 *      I S C _ g e t _ h o s t
 *
 **************************************
 *
 * Functional description
 *      Get host name in non-plain buffer.
 *
 **************************************/
	TEXT buffer[BUFFER_SMALL];
	ISC_get_host(buffer, sizeof(buffer));
	host = buffer;
	return host.c_str();
}

#ifdef UNIX
bool ISC_get_user(Firebird::string*	name,
				  int*	id,
				  int*	group,
				  const TEXT*	user_string)
{
/**************************************
 *
 *      I S C _ g e t _ u s e r   ( U N I X )
 *
 **************************************
 *
 * Functional description
 *      Find out who the user is.
 *
 **************************************/
/* egid and euid need to be signed, uid_t is unsigned on SUN! */
	SLONG egid, euid;
	TEXT user_name[256];
	const TEXT* p = 0;

	if (user_string && *user_string) {
		const TEXT* q = user_string;
		char* un;
		for (un = user_name; (*un = *q++) && *un != '.'; un++);
		*un = 0;
		p = user_name;
		egid = euid = -1;
#ifdef TRUST_CLIENT_VERIFICATION
		if (*q) {
			egid = atoi(q);
			while (*q && (*q != '.'))
				q++;
			if (*q == '.') {
				q++;
				euid = atoi(q);
			}
		}
#endif
	}
	else {
		euid = (SLONG) geteuid();
		egid = (SLONG) getegid();
		const struct passwd* password = getpwuid(euid);
		if (password)
			p = password->pw_name;
		else
			p = "";
		endpwent();
	}

	if (name)
		*name = p;

	if (id)
		*id = euid;

	if (group)
		*group = egid;

	return (euid == 0);
}
#endif


#ifdef WIN_NT
bool ISC_get_user(Firebird::string*	name,
				  int*	id,
				  int*	group,
				  const TEXT*	user_string)
{
/**************************************
 *
 *      I S C _ g e t _ u s e r   ( W I N _ N T )
 *
 **************************************
 *
 * Functional description
 *      Find out who the user is.
 *
 **************************************/
	if (id)
		*id = -1;

	if (group)
		*group = -1;

	if (name)
	{
		DWORD name_len = UNLEN;
		TEXT* nm = name->getBuffer(name_len + 1);
		if (GetUserName(nm, &name_len))
		{
			nm[name_len] = 0;

			// NT user name is case insensitive
			CharUpperBuff(nm, name_len);
			name->recalculate_length();
		}
		else
		{
			*name = "";
		}
	}

	return false;
}
#endif //WIN_NT

inline void setPrefixIfNotEmpty(const Firebird::PathName& prefix, SSHORT arg_type)
{
/**************************************
 *
 *         s e t P r e f i x I f N o t E m p t y
 *
 **************************************
 *
 * Functional description
 *      Helper for ISC_set_prefix
 *
 **************************************/
	if (prefix.hasData())
	{
		// ignore here return value of gds__get_prefix():
		// it will never fail with our good arguments
		gds__get_prefix(arg_type, prefix.c_str());
	}
}

SLONG ISC_set_prefix(const TEXT* sw, const TEXT* path)
{
/**************************************
 *
 *      i s c _ s e t _ p r e f i x
 *
 **************************************
 *
 * Functional description
 *      Parse the 'E' argument further for 'EL' 'EM' or 'E'
 *
 **************************************/

	/*
	 * We can't call gds__get_prefix() at once when switch is found.
	 * gds__get_prefix() invokes gdsPrefixInit(), which in turn causes
	 * config file to be loaded. And in case when -el or -em is given
	 * before -e, this leads to use of wrong firebird.conf.
	 * To avoid it accumulate values for switches locally,
	 * and finally when called with sw==0, use them in correct order.
	 */
	static struct ESwitches
	{
		Firebird::PathName prefix, lockPrefix, msgPrefix;

		explicit ESwitches(MemoryPool& p)
			: prefix(p), lockPrefix(p), msgPrefix(p)
		{
		}
	}* eSw = 0;

	if (! sw)
	{
		if (eSw)
		{
			setPrefixIfNotEmpty(eSw->prefix, IB_PREFIX_TYPE);
			setPrefixIfNotEmpty(eSw->lockPrefix, IB_PREFIX_LOCK_TYPE);
			setPrefixIfNotEmpty(eSw->msgPrefix, IB_PREFIX_MSG_TYPE);

			delete eSw;
			eSw = 0;
		}

		return 0;
	}

	if ((!path) || (path[0] <= ' '))
	{
		return -1;
	}

	if (! eSw)
	{
		eSw = FB_NEW(*getDefaultMemoryPool()) ESwitches(*getDefaultMemoryPool());
	}

	switch (UPPER(*sw))
	{
	case '\0':
		eSw->prefix = path;
		break;
	case 'L':
		eSw->lockPrefix = path;
		break;
	case 'M':
		eSw->msgPrefix = path;
		break;
	default:
		return -1;
	}

	return 0;
}


#ifdef UNIX
#ifdef AIX_PPC
#define _UNIX95
#endif
#include <grp.h>
#ifdef AIX_PPC
#undef _UNIX95
#endif

SLONG ISC_get_user_group_id(const TEXT* user_group_name)
{
/**************************************
 *
 *      I S C _ g e t _ u s e r _ g r o u p _ n a n e
 *
 **************************************
 *
 * Functional description
 *
 *      Return user group id if user group name found,
 *      Otherwise return 0.
 *
 *                  ---  for UNIX platform  ---
 *
 **************************************/
	SLONG n = 0;

	const struct group* user_group = getgrnam(user_group_name);
	if (user_group)
		n = user_group->gr_gid;

	return (n);
}
#else
SLONG ISC_get_user_group_id(const TEXT* user_group_name)
{
/**************************************
 *
 *      I S C _ g e t _ u s e r _ g r o u p _ n a n e
 *
 *                  ---  for non-UNIX platform  ---
 *
 **************************************
 *
 * Functional description
 *
 *      needed to be developed for non-unix platform
 *
 *      Currently it just returns 0.
 *
 **************************************/

	SLONG n = 0;
	return (n);
}
#endif /* end of ifdef UNIX */


#ifdef WIN_NT
LPSECURITY_ATTRIBUTES ISC_get_security_desc()
{
	return security_attributes();
}
#endif


void iscLogStatus(const TEXT* text, const ISC_STATUS* status_vector)
{
/**************************************
 *
 *	g d s _ $ l o g _ s t a t u s
 *
 **************************************
 *
 * Functional description
 *	Log error to error log.
 *
 **************************************/
	fb_assert(status_vector[1] != FB_SUCCESS);

	try
	{
		Firebird::string buffer(text ? text : "");

		TEXT temp[BUFFER_LARGE];
		while (fb_interpret(temp, sizeof(temp), &status_vector))
		{
			if (!buffer.isEmpty())
			{
				buffer += "\n\t";
			}
			buffer += temp;
		}

		gds__log("%s", buffer.c_str());
	}
	catch (const Firebird::Exception&)
	{} // no-op
}


void iscLogException(const char* text, const Firebird::Exception& e)
{
/**************************************
 *
 *      l o g E x c e p t i o n
 *
 **************************************
 *
 * Functional description
 *	Add record about an exception to firebird.log
 *
 **************************************/
	ISC_STATUS_ARRAY s;
	e.stuff_exception(s);
	iscLogStatus(text, s);
}

