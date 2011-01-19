/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		ConfigCache.cpp
 *	DESCRIPTION:	Base for class, representing cached config file.
 *
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
 *  Copyright (c) 2010 Alexander Peshkoff <peshkoff@mail.ru>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 *
 */

#include "../common/config/ConfigCache.h"
#include "../common/config/config_file.h"

#include <sys/types.h>
#include <sys/stat.h>

#ifndef WIN_NT
#include <unistd.h>
#else
#include <errno.h>
#endif

ConfigCache::ConfigCache(Firebird::MemoryPool& p, const Firebird::PathName& fName)
	: PermanentStorage(p), fileName(getPool(), fName), fileTime(0)
{
}

ConfigCache::~ConfigCache()
{
}

void ConfigCache::checkLoadConfig()
{
	time_t newTime = getTime();
	if (fileTime == newTime)
	{
		return;
	}

	Firebird::WriteLockGuard guard(rwLock);

	// may be someone already reloaded?
	newTime = getTime();
	if (fileTime == newTime)
	{
		return;
	}

	fileTime = newTime;

	loadConfig();
}

time_t ConfigCache::getTime()
{
	struct stat st;
	if (stat(fileName.c_str(), &st) != 0)
	{
		if (errno == ENOENT)
		{
			// config file is missing, but this is not our problem
			return 0;
		}
		Firebird::system_call_failed::raise("stat");
	}
	return st.st_mtime;
}
