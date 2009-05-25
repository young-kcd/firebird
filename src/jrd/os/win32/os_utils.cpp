/*
 *  The contents of this file are subject to the Initial
 *  Developer's Public License Version 1.0 (the "License");
 *  you may not use this file except in compliance with the
 *  License. You may obtain a copy of the License at
 *  http://www.ibphoenix.com/main.nfs?a=ibphoenix&page=ibp_idpl.
 *
 *  Software distributed under the License is distributed AS IS,
 *  WITHOUT WARRANTY OF ANY KIND, either express or implied.
 *  See the License for the specific language governing rights
 *  and limitations under the License.
 *
 *  The Original Code was created by Alexander Peshkoff
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2009 Alexander Peshkoff <peshkoff@mail.ru>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 *
 */


// =====================================
// File functions

#include "firebird.h"
#include "../jrd/common.h"

#include "../common/classes/init.h"
#include "../jrd/gdsassert.h"
#include "../jrd/os/os_utils.h"
#include "../jrd/constants.h"
#include "../jrd/os/path_utils.h"

#include <direct.h>
#include <io.h> // isatty()
#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <fcntl.h>

namespace os_utils
{

// waits for implementation
SLONG get_user_group_id(const TEXT* /*user_group_name*/)
{
	return 0;
}


// waits for implementation
SLONG get_user_id(const TEXT* /*user_name*/)
{
	return -1;
}


// waits for implementation
bool get_user_home(int /*user_id*/, Firebird::PathName& /*homeDir*/)
{
	return false;
}

// create directory for lock files and set appropriate access rights
void createLockDirectory(const char* pathname)
{
	static bool errorLogged = false;

	DWORD attr = GetFileAttributes(pathname);
	DWORD errcode = 0;
	if (attr == INVALID_FILE_ATTRIBUTES)
	{
		errcode = GetLastError();
		if (errcode == ERROR_FILE_NOT_FOUND)
		{
			if (!CreateDirectory(pathname, NULL)) {
				errcode = GetLastError();
			}
			else
			{
				attr = GetFileAttributes(pathname);
				if (attr == INVALID_FILE_ATTRIBUTES) {
					errcode = GetLastError();
				}
			}
		}
	}

	Firebird::string err;
	if (attr == INVALID_FILE_ATTRIBUTES)
	{
		err.printf("Can't create directory \"%s\". OS errno is %d", pathname, errcode);
		if (!errorLogged)
		{
			errorLogged = true;
			gds__log(err.c_str());
		}
		Firebird::system_call_failed::raise(err.c_str(), errcode);
	}

	if (!(attr & FILE_ATTRIBUTE_DIRECTORY))
	{
		err.printf("Can't create directory \"%s\". File with same name already exists", pathname);
		if (!errorLogged)
		{
			errorLogged = true;
			gds__log(err.c_str());
		}
		Firebird::fatal_exception::raise(err.c_str());
	}

	if (attr & FILE_ATTRIBUTE_READONLY)
	{
		err.printf("Can't create directory \"%s\". Readonly directory with same name already exists", pathname);
		if (!errorLogged)
		{
			errorLogged = true;
			gds__log(err.c_str());
		}
		Firebird::fatal_exception::raise(err.c_str());
	}
}

// open (or create if missing) and set appropriate access rights
int openCreateShmemFile(const char* pathname, int flags)
{
	return ::open(pathname, flags | O_RDWR | O_CREAT, S_IREAD | S_IWRITE);
}

} // namespace os_utils
