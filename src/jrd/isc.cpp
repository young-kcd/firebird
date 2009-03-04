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
#include "../jrd/isc.h"
#include "../jrd/ibase.h"
#include "../jrd/jrd.h"
#include "../jrd/scl.h"
#include "../jrd/gds_proto.h"
#include "../jrd/isc_proto.h"
#include "../jrd/jrd_proto.h"

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

static USHORT os_type;
static SECURITY_ATTRIBUTES security_attr;

#endif // WIN_NT

static TEXT user_name[256];
static USHORT ast_count;


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
#include "../jrd/prv_m_bypass.h"

#define LOGICAL_NAME_TABLE      "LNM$FILE_DEV"
#define WAKE_LOCK               "gds__process_%d"

static POKE pokes;
static lock_status wake_lock;

#endif /* of ifdef VMS */

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

#ifdef VMS
static void blocking_ast(void);
static void poke_ast(POKE);
static int wait_test(SSHORT *);
#endif

#ifndef REQUESTER
void ISC_ast_enter(void)
{
/**************************************
 *
 *      I S C _ a s t _ e n t e r
 *
 **************************************
 *
 * Functional description
 *      Enter ast mode.
 *
 **************************************/

	++ast_count;
}
#endif


#ifndef REQUESTER
void ISC_ast_exit(void)
{
/**************************************
 *
 *      I S C _ a s t _ e x i t
 *
 **************************************
 *
 * Functional description
 *      Enter ast mode.
 *
 **************************************/

	--ast_count;
}
#endif


bool ISC_check_process_existence(SLONG	pid,
								SLONG	xl_pid,
								bool	super_user)
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

#if defined(UNIX)
	return (kill((int) pid, 0) == -1 &&
			(errno == ESRCH
			 || (super_user && errno == EPERM)) ? false : true);
#elif defined(VMS)
	ULONG item = JPI$_PID;
	return (lib$getjpi(&item, &pid, NULL, NULL, NULL, NULL) == SS$_NONEXPR) ?
		false : true;
#elif defined(WIN_NT)
	HANDLE handle = OpenProcess(SYNCHRONIZE, FALSE, (DWORD) pid);

	if (!handle)
	{
		return (GetLastError() == ERROR_ACCESS_DENIED);
	}

	const bool alive = (WaitForSingleObject(handle, 0) != WAIT_OBJECT_0);
	CloseHandle(handle);
	return alive;
#else
	return true;
#endif
}


#ifdef VMS
int ISC_expand_logical_once(const TEXT* file_name,
							USHORT file_length, TEXT* expanded_name, USHORT bufsize)
{
/**************************************
 *
 *      I S C _ e x p a n d _ l o g i c a l _ o n c e
 *
 **************************************
 *
 * Functional description
 *      Expand a logical name.  If it doesn't exist, return 0.
 *
 **************************************/
	struct dsc$descriptor_s desc1, desc2;

	if (!file_length)
		file_length = strlen(file_name);

	ISC_make_desc(file_name, &desc1, file_length);
	ISC_make_desc(LOGICAL_NAME_TABLE, &desc2, sizeof(LOGICAL_NAME_TABLE) - 1);

	USHORT l;
	ITM items[2];
	items[0].itm_length = bufsize; //256;
	items[0].itm_code = LNM$_STRING;
	items[0].itm_buffer = expanded_name;
	items[0].itm_return_length = &l;

	items[1].itm_length = 0;
	items[1].itm_code = 0;

	int attr = LNM$M_CASE_BLIND;

	if (!(sys$trnlnm(&attr, &desc2, &desc1, NULL, items) & 1)) {
		while (file_length--) {
			if (bufsize-- == 1)
				break;
			*expanded_name++ = *file_name++;
		}
		*expanded_name = 0;
		return 0;
	}

	expanded_name[l] = 0;

	return l;
}
#endif


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

#elif defined (VMS)

TEXT* ISC_get_host(TEXT* string, USHORT length)
{
/**************************************
 *
 *      I S C _ g e t _ h o s t                 ( V M S )
 *
 **************************************
 *
 * Functional description
 *      Get host name.
 *
 **************************************/
	if (!ISC_expand_logical_once("SYS$NODE", sizeof("SYS$NODE") - 1, string, length))
		strcpy(string, "local");
	else {
		TEXT* p = string;
		if (*p == '_')
			++p;

		for (; *p && *p != ':'; p++)
			if (*p >= 'A' && *p <= 'Z')
				*p = *p - 'A' + 'a';

		*p = 0;
	}

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


#ifdef VMS
bool ISC_get_user(
									  TEXT* name,
									  int* id,
									  int* group,
									  TEXT* project,
									TEXT* organization,
									int* node,
									const TEXT* user_string)
{
/**************************************
 *
 *      I S C _ g e t _ u s e r   ( V M S )
 *
 **************************************
 *
 * Functional description
 *      Find out who the user is.
 *
 **************************************/
	SLONG privileges[2];
	USHORT uic[2];
	TEXT user_name[256];

	if (user_string && *user_string) {
		const TEXT* q = user_string;
		TEXT* p;
		for (p = user_name; (*p = *q++) && *p != '.'; p++);
		*p = 0;
		uic[0] = uic[1] = -1;
		if (*q) {
			uic[1] = atoi(q);
			while (*q && (*q != '.'))
				q++;
			if (*q == '.') {
				q++;
				uic[0] = atoi(q);
			}
		}
		privileges[0] = 0;
		if (name) {
			for (p = user_name; *p && *p != ' ';)
				*name++ = *p++;
			*name = 0;
		}
	}
	else {
		USHORT len0, len1, len2;
	    ITM items[4];
		items[0].itm_code = JPI$_UIC;
		items[0].itm_length = sizeof(uic);
		items[0].itm_buffer = uic;
		items[0].itm_return_length = &len0;

		items[1].itm_code = JPI$_USERNAME;
		items[1].itm_length = sizeof(user_name);
		items[1].itm_buffer = user_name;
		items[1].itm_return_length = &len1;

		items[2].itm_code = JPI$_CURPRIV;
		items[2].itm_length = sizeof(privileges);
		items[2].itm_buffer = privileges;
		items[2].itm_return_length = &len2;

		items[3].itm_code = 0;
		items[3].itm_length = 0;

		const SLONG status = sys$getjpiw(NULL, NULL, NULL, items, NULL, NULL, NULL);

		if (!(status & 1)) {
			len1 = 0;
			uic[0] = uic[1] = 0;
		}

		user_name[len1] = 0;

		if (name) {
			for (const TEXT* p = user_name; *p && *p != ' ';)
				*name++ = *p++;
			*name = 0;
		}
	}

	if (id)
		*id = uic[0];

	if (group)
		*group = uic[1];

	if (project)
		*project = 0;

	if (organization)
		*organization = 0;

	if (node)
		*node = 0;

	return (privileges[0] & (SLONG) PRV$M_BYPASS);
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

#ifdef VMS
int ISC_make_desc(const TEXT* string, struct dsc$descriptor* desc, USHORT length)
{
/**************************************
 *
 *      I S C _ m a k e _ d e s c
 *
 **************************************
 *
 * Functional description
 *      Make a VMS string descriptor out of a string.  Return 
 *      length of string.
 *
 **************************************/

	desc->dsc$b_dtype = DSC$K_DTYPE_T;
	desc->dsc$b_class = DSC$K_CLASS_S;
	// CVC: I assume the non-const condition would be needed, can't check.
	desc->dsc$a_pointer = const_cast<TEXT*>(string);

	if (length)
		desc->dsc$w_length = length;
	else {
		desc->dsc$w_length = 0;
		while (*string++)
			++desc->dsc$w_length;
	}

	return desc->dsc$w_length;
}
#endif

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
	static struct ESwitches {
		Firebird::PathName prefix, lockPrefix, msgPrefix;
		ESwitches(MemoryPool& p) : prefix(p), lockPrefix(p), msgPrefix(p) { }
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

	switch(UPPER(*sw))
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


void ISC_prefix(TEXT* string, const TEXT* root)
{
/**************************************
 *
 *      i s c _ $ p r e f i x   
 *
 **************************************
 *
 * Functional description
 *      Wrapper function for gds__prefix.
 *
 **************************************/

	gds__prefix(string, root);
	return;
}


void ISC_prefix_lock(TEXT* string, const TEXT* root)
{
/**************************************
 *
 *      i s c _ $ p r e f i x _ l o c k
 *
 **************************************
 *
 * Functional description
 *      Wrapper function for gds__prefix_lock.
 *
 **************************************/

	gds__prefix_lock(string, root);
	return;
}


void ISC_prefix_msg(TEXT* string, const TEXT* root)
{
/**************************************
 *
 *      i s c _ $ p r e f i x _ m s g  
 *
 **************************************
 *
 * Functional description
 *      Wrapper function for gds__prefix_msg.
 *
 **************************************/

	gds__prefix_msg(string, root);
	return;
}


#ifndef REQUESTER
void ISC_set_user(const TEXT* string)
{
/**************************************
 *
 *      I S C _ s e t _ u s e r
 *
 **************************************
 *
 * Functional description
 *      Set user name for feeble operating system that don't
 *      support the concept, or support it badly.
 *
 **************************************/
// CVC: And including a buffer overflow, too?
// Using static data, not thread safe. Probably deprecated?
	strncpy(user_name, string, sizeof(user_name));
	user_name[sizeof(user_name) - 1] = 0;
}
#endif


#ifdef VMS
void ISC_wait(SSHORT * iosb, SLONG event_flag)
{
/**************************************
 *
 *      I S C _ w a i t         ( V M S )
 *
 **************************************
 *
 * Functional description
 *      Wait for a asynchronous VMS event.
 *
 **************************************/

	if (*iosb)
		return;

/* If we're at AST state, and we're not running under
   a AST invoked thread, wait locally. */

	if (ast_count && !gds__ast_active()) {
		sys$synch(event_flag, iosb);
		return;
	}

	gds__thread_wait(wait_test, iosb);
}
#endif


#ifdef VMS
void ISC_wake(SLONG process_id)
{
/**************************************
 *
 *      I S C _ w a k e
 *
 **************************************
 *
 * Functional description
 *      Wake up another VMS process.  Process can be local or
 *      remote (but on the same CPU).
 *
 **************************************/

/* Try to do a simple wake.  If this succeeds, we're done. */

	int status = sys$wake(&process_id, 0);
#ifdef __ALPHA
	THREAD_wakeup();
#endif

	if (status & 1)
		return;

/* Do wake by siezing a lock, delivering a blocking ast */

/* Find a free poke block to use */

	POKE poke;
	for (poke = pokes; poke; poke = poke->poke_next)
		if (!poke->poke_use_count)
			break;

	if (!poke) {
		poke = (POKE) gds__alloc((SLONG) sizeof(struct poke));
		/* FREE: apparently never freed */
		if (!poke) {			/* NOMEM: */
			DEV_REPORT("ISC_wake: out of memory condition");
			return;				/* error handling too difficult here */
		}
		poke->poke_next = pokes;
		pokes = poke;
	}

	++poke->poke_use_count;

	TEXT string[32];
	struct dsc$descriptor_s desc;
	sprintf(string, WAKE_LOCK, process_id);
	ISC_make_desc(string, &desc, 0);

	status = sys$enq(0,			/* event flag */
					 LCK$K_PWMODE,	/* lock mode */
					 &poke->poke_lksb,	/* Lock status block */
					 LCK$M_SYSTEM,	/* flags */
					 &desc,		/* resource name */
					 0,			/* parent id */
					 poke_ast,	/* ast address */
					 poke,		/* ast argument */
					 0,			/* blocking ast */
					 0,			/* access mode */
					 0);
}
#endif


#ifdef VMS
void ISC_wake_init(void)
{
/**************************************
 *
 *      I S C _ w a k e _ i n i t
 *
 **************************************
 *
 * Functional description
 *      Set up to be awakened by another process thru a blocking AST.
 *
 **************************************/
	TEXT string[32];
	struct dsc$descriptor_s desc;

/* If we already have lock, we're done */

	if (wake_lock.lksb_lock_id)
		return;

	sprintf(string, WAKE_LOCK, getpid());
	ISC_make_desc(string, &desc, 0);

	int status = sys$enqw(0,		/* event flag */
					  LCK$K_PWMODE,	/* lock mode */
					  &wake_lock,	/* Lock status block */
					  LCK$M_SYSTEM,	/* flags */
					  &desc,	/* resource name */
					  0,		/* parent id */
					  0,		/* ast address */
					  0,		/* ast argument */
					  blocking_ast,	/* blocking ast */
					  0,		/* access mode */
					  0);
}
#endif


#ifdef VMS
static void blocking_ast(void)
{
/**************************************
 *
 *      b l o c k i n g _ a s t
 *
 **************************************
 *
 * Functional description
 *      Somebody else is trying to post a lock.
 *
 **************************************/

/* Initially down grade the lock to let the other guy complete */

	int status = sys$enqw(0,		/* event flag */
					  LCK$K_NLMODE,	/* lock mode */
					  &wake_lock,	/* Lock status block */
					  LCK$M_CONVERT,	/* flags */
					  0,		/* resource name */
					  0,		/* parent id */
					  0,		/* ast address */
					  0,		/* ast argument */
					  0,		/* blocking ast */
					  0,		/* access mode */
					  0);

/* Wake us up */

	status = sys$wake(0, 0);

/* Now try to upgrade lock */

	status = sys$enq(0,			/* event flag */
					 LCK$K_PRMODE,	/* lock mode */
					 &wake_lock,	/* Lock status block */
					 LCK$M_CONVERT,	/* flags */
					 0,			/* resource name */
					 0,			/* parent id */
					 0,			/* ast address */
					 0,			/* ast argument */
					 blocking_ast,	/* blocking ast */
					 0,			/* access mode */
					 0);
}
#endif


#ifdef VMS
static void poke_ast(POKE poke)
{
/**************************************
 *
 *      p o k e _ a s t
 *
 **************************************
 *
 * Functional description
 *      Lock to poke event has completed.  Update value block
 *      and deque the lock.
 *
 **************************************/
	lock_status* lksb = &poke->poke_lksb;
	int status = sys$deq(lksb->lksb_lock_id, 0, 0, 0);
	--poke->poke_use_count;
}
#endif


#ifdef VMS
static int wait_test(SSHORT * iosb)
{
/**************************************
 *
 *      w a i t _ t e s t
 *
 **************************************
 *
 * Functional description
 *      Return true if an I/O event is complete.
 *
 **************************************/

	return *iosb;
}
#endif


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
// Returns the type of OS: true for NT,
// false for the 16-bit based ones (9x/ME, ...).
//
bool ISC_is_WinNT()
{
	// thread safe??? :-)
	if (!os_type)
	{
		os_type = 1;			/* Default to NT */
		/* The first time this routine is called we use the Windows API
		   call GetVersion to determine whether Windows/NT or Chicago
		   is running. */
		OSVERSIONINFO OsVersionInfo;
		
		OsVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		if (GetVersionEx((LPOSVERSIONINFO) & OsVersionInfo))
		{
		 if (OsVersionInfo.dwPlatformId != VER_PLATFORM_WIN32_NT)
			os_type = 2;
		}

	}

	return (os_type != 2);
}

//____________________________________________________________
//
//
LPSECURITY_ATTRIBUTES ISC_get_security_desc()
{
	if (security_attr.lpSecurityDescriptor)
	{
		return &security_attr;
	}

	// This is our first call. Ensure that our process has
	// the SYNCHRONIZE privilege granted to everyone.

	PACL pOldACL = NULL;
	GetSecurityInfo(GetCurrentProcess(), SE_KERNEL_OBJECT,
					DACL_SECURITY_INFORMATION,
					NULL, NULL, &pOldACL, NULL, NULL);

	// NULL pOldACL means all privileges. If we assign pNewACL in this case 
	// we'll lost all privileges except assigned SYNCHRONIZE
	if (pOldACL) 
	{
		SID_IDENTIFIER_AUTHORITY SIDAuth = SECURITY_WORLD_SID_AUTHORITY;
		PSID pSID = NULL;
		AllocateAndInitializeSid(&SIDAuth, 1, SECURITY_WORLD_RID,
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

	// Create and initialize the default security descriptor
	// to be assigned to various IPC objects.
	//
	// WARNING!!! The absent DACL means full access granted
	// to everyone, this is a huge security risk!

	PSECURITY_DESCRIPTOR p_security_desc =
		(PSECURITY_DESCRIPTOR) gds__alloc(SECURITY_DESCRIPTOR_MIN_LENGTH);
/* FREE: apparently never freed */
	if (!p_security_desc)		/* NOMEM: */
	{
		return NULL;
	}
#ifdef DEBUG_GDS_ALLOC
	gds_alloc_flag_unfreed((void *) p_security_desc);
#endif

	if (!InitializeSecurityDescriptor(p_security_desc,
									  SECURITY_DESCRIPTOR_REVISION) ||
		!SetSecurityDescriptorDacl(p_security_desc, TRUE, NULL, FALSE))
	{
		gds__free(p_security_desc);
		return NULL;
	}

	security_attr.nLength = sizeof(security_attr);
	security_attr.lpSecurityDescriptor = p_security_desc;
	security_attr.bInheritHandle = TRUE;

	return &security_attr;
}

#endif
