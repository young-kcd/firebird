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

#include <string.h>
#include "firebird.h"
#include "DirectoryList.h"
#include "AdminException.h"

#ifdef _WIN32
#ifndef strcasecmp
#define strcasecmp		stricmp
#define strncasecmp		strnicmp
#endif
#endif


DirectoryList::DirectoryList(const char* options)
{
	const char *p;
	
	for (p = options; *p && *p != ' '; ++p)
		;
	
	JString option(options, p - options);
	
	while (*p == ' ')
		++p;
	
	parse(option, p);
}

DirectoryList::DirectoryList(const char *option, const char* list)
{
	parse(option, list);
}

DirectoryList::~DirectoryList(void)
{
	if (strings)
		delete [] strings;
}

bool DirectoryList::validateFilename(const char* fileName)
{
	if (!enabled)
		return false;
	
	if (!restricted)
		return true;
	
	for (int n = 0; n < numberDirectories; ++n)
		if (matchFileName (directories [n], fileName))
			return true;
	
	return false;		
}

bool DirectoryList::matchFileName(const char* directory, const char* fileName)
{
	return true;
}


void DirectoryList::parse(const char* option, const char* list)
{
	numberDirectories = 0;
	strings = NULL;
	
	if (strcasecmp (option, "None") == 0)
		enabled = false;
	else if (strcasecmp (option, "Full") == 0)
		{
		enabled = true;
		restricted = false;
		}
	else if (strcasecmp (option, "Restrict") == 0)
		{
		enabled = true;
		restricted = true;
		strings = new char [strlen (list) + 1];
		char *p = strings;
		for (const char *q = list; *q;)
			{
			if (numberDirectories >= MAX_DIRECTORIES)
				throw AdminException ("maximum number of directories exceeded in configuration file");
			directories [numberDirectories++] = p;
			while (*q && *q != ';')
				*p++ = *q++;
			*p++ = 0;
			while (*q == ';')
				++q;
			}
		}
	else
		throw AdminException ("expected directory options \"none\", \"full\", or \"restrict\", got \"%s\"", option);
}
