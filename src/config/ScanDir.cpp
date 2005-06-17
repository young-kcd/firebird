/*
 *  
 *     The contents of this file are subject to the Initial 
 *     Developer's Public License Version 1.0 (the "License"); 
 *     you may not use this file except in compliance with the 
 *     License. You may obtain a copy of the License at 
 *     http://www.ibphoenix.com/idpl.html. 
 *
 *     Software distributed under the License is distributed on 
 *     an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either 
 *     express or implied.  See the License for the specific 
 *     language governing rights and limitations under the License.
 *
 *     The contents of this file or any work derived from this file
 *     may not be distributed under any other license whatsoever 
 *     without the express prior written permission of the original 
 *     author.
 *
 *
 *  The Original Code was created by James A. Starkey for IBPhoenix.
 *
 *  Copyright (c) 1997 - 2000, 2001, 2003 James A. Starkey
 *  Copyright (c) 1997 - 2000, 2001, 2003 Netfrastructure, Inc.
 *  All Rights Reserved.
 */

// ScanDir.cpp: implementation of the ScanDir class.
//
//////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <string.h>

#ifndef _WIN32
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

#include "firebird.h"
#include "../jrd/common.h"
#include "ScanDir.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ScanDir::ScanDir(const char *direct, const char *pat)
{
	directory = direct;
	pattern = pat;
#ifdef _WIN32
	handle = NULL;
#else
	dir = opendir (direct);
#endif
}

ScanDir::~ScanDir()
{
#ifdef _WIN32
	if (handle)
		FindClose (handle);
#else
	if (dir)
		closedir (dir);
#endif
}

bool ScanDir::next()
{
#ifdef _WIN32
	if (handle == NULL)
		{
		handle = FindFirstFile (directory + "\\" + pattern, &data);
		return handle != INVALID_HANDLE_VALUE;
		}

	return FindNextFile (handle, &data) != 0;
#else
	if (!dir)
		return false;

	while (data = readdir (dir))
		{
		if (match (pattern, data->d_name))
			return true;
		}

	return false;
#endif
}

const char* ScanDir::getFileName()
{
#ifdef _WIN32
	fileName = data.cFileName;
#else
	fileName = data->d_name;
#endif

	return fileName;
}


const char* ScanDir::getFilePath()
{
#ifdef _WIN32
	filePath.Format("%s\\%s", (const char*) directory, data.cFileName);
#else
	filePath.Format("%s/%s", (const char*) directory, data->d_name);
#endif

	return filePath;
}

bool ScanDir::match(const char *pattern, const char *name)
{
	if (*pattern == '*')
		{
		if (!pattern [1])
			return true;
		for (const char *p = name; *p; ++p)
			if (match (pattern + 1, p))
				return true;
		return false;
		}

	if (*pattern != *name)
		return false;

	if (!*pattern)
		return true;

	return match (pattern + 1, name + 1);
}

bool ScanDir::isDirectory()
{
#ifdef _WIN32
	return (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
#else
#ifndef SOLARIS
	if (data->d_type == DT_DIR)
		return true;
#endif

	struct stat buf;

    if (stat (getFilePath(), &buf))
		return false;

	return S_ISDIR (buf.st_mode);
#endif
}

bool ScanDir::isDots()
{
	return getFileName() [0] == '.';
}
