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

// AdminException.cpp: implementation of the AdminException class.
//
//////////////////////////////////////////////////////////////////////

#include "firebird.h"
#include <stdio.h>
#include <stdarg.h>
#include "AdminException.h"

#ifdef _WIN32
#define vsnprintf	_vsnprintf
#define snprintf	_snprintf
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

AdminException::AdminException(const char *txt, ...)
{
	va_list		args;
	va_start	(args, txt);
	char		temp [1024];

	int ret = vsnprintf (temp, sizeof (temp) - 1, txt, args);
	
	if (ret < 0)
		{
		int l = sizeof (temp) * 2;
		char *buffer = new char [l];
		vsnprintf (buffer, l, txt, args);
		text = buffer;
		delete [] buffer;
		}
	else if (ret >= sizeof (temp))
		{
		int l = ret + 1;
		char *buffer = new char [l];
		vsnprintf (buffer, l, txt, args);
		text = buffer;
		delete [] buffer;
		}
	else
		text = temp;

	text = temp;
	lineNumber;
}

AdminException::~AdminException()
{

}

const char* AdminException::getText()
{
	return text;
}

void AdminException::setLocation(JString file, int line)
{
	fileName = file;
	lineNumber = line;
	char	temp [1024];
	char	*buffer = temp;
	int		l = sizeof (temp);
	
	for (int n =0; n < 3; ++n)
		{
		int ret = snprintf (buffer, l, "%s, line %d: %s", 
							(const char*) fileName, lineNumber, (const char*) text);
		if (ret < 0)
			l += sizeof(temp);
		else if (ret >= l)
			l = ret + 1;
		else
			{
			text = buffer;
			break;
			}
		if (text != buffer)
			delete [] buffer;
		buffer = new char [l];
		}
	
	if (buffer != temp)
		delete [] buffer;
}
