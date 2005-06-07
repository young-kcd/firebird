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

// InputFile.cpp: implementation of the InputFile class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _WIN32
#include <unistd.h>
#else
#include <io.h> // for unlink
#endif

#include <stdio.h>
#include <string.h>
#include "firebird.h"
#include "InputFile.h"
#include "AdminException.h"

#define ISLOWER(c)			(c >= 'a' && c <= 'z')
#define ISUPPER(c)			(c >= 'A' && c <= 'Z')
#define ISDIGIT(c)			(c >= '0' && c <= '9')

#ifndef UPPER
#define UPPER(c)			((ISLOWER (c)) ? c - 'a' + 'A' : c)
#endif

#define MAX_FILE_NAME	256
#define BACKUP_FILES	5

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

InputFile::InputFile(const char *name)
{
	changes = NULL;
	
	if (!openInputFile (name))
		throw AdminException ("can't open file \"%s\"", name);
}

InputFile::InputFile(void)
{
	changes = NULL;
}

InputFile::~InputFile()
{
	close();

	for (FileChange *change; change = changes;)
		{
		changes = change->next;
		delete change;
		}
}

void InputFile::close()
{
	if (file)
		{
		fclose ((FILE*) file);
		file = NULL;
		}
}

const char* InputFile::getSegment()
{
	if (!file)
		throw AdminException ("file has been closed");

	if (!fgets (buffer, sizeof (buffer), (FILE*) file))
		return NULL;

	//++lineNumber;
	segmentLength = (int) strlen (buffer);

	return buffer;
}

const char* InputFile::getFileName()
{
	return fileName;
}

InputFile* InputFile::getInputFile()
{
	return this;
}

void InputFile::postChange(int line, int skip, JString insertion)
{
	FileChange *change = new FileChange;
	change->lineNumber = line;
	change->linesSkipped = skip;
	change->insertion = insertion;
	
	for (FileChange **p = &changes;; p = &(*p)->next)
		{
		FileChange *next = *p;
		if (!next || next->lineNumber > change->lineNumber)
			{
			change->next = *p;
			*p = change;
			break;
			}
		}
}

void InputFile::rewrite()
{
	FILE *input = fopen (fileName, "r");

	if (!input)
		throw AdminException ("can't open \"%s\" for input", (const char*) fileName);

	char tempName [MAX_FILE_NAME];
	sprintf (tempName, "%s.tmp", (const char*) fileName);
	FILE *output = fopen (tempName,"w");

	if (!output)
		throw AdminException ("can't open \"%s\" for output", tempName);

	char temp [1024];
	int line = 0;

	for (FileChange *change = changes; change; change = change->next)
		{
		for (; line < change->lineNumber; ++line)
			if (fgets (temp, sizeof (temp), input))
				fputs (temp, output);
		//fputs ("#insertion starts here\n", output);
		fputs (change->insertion, output);
		//fputs ("#insertion end here\n", output);
		for (int n = 0; n < change->linesSkipped; ++n)
			fgets (temp, sizeof (temp), input);
		line += change->linesSkipped;
		}

	while (fgets (temp, sizeof (temp), input))
		fputs (temp, output);

	fclose (input);
	fclose (output);
	char filename1 [MAX_FILE_NAME];
	char filename2 [MAX_FILE_NAME];

	for (int n = BACKUP_FILES; n >= 0; --n)
		{
		sprintf (filename1, "%s.%d", (const char*) fileName, n);
		if (n)
			sprintf (filename2, "%s.%d", (const char*) fileName, n - 1);
		else
			strcpy (filename2, fileName);
		if (n == BACKUP_FILES)
			unlink (filename1);
		rename (filename2, filename1);
		}

	if (rename (tempName, fileName))
		perror ("rename");
}

bool InputFile::pathEqual(const char *path1, const char *path2)
{
#ifdef _WIN32
	for (; *path1 && *path2; ++path1, ++path2)
		if (*path1 != *path2 &&
		    UPPER (*path1) != UPPER (*path2) &&
			!((*path1 == '/' || *path1 == '\\') &&
			  (*path2 == '/' || *path2 == '\\')))
			return false;
#else
	for (; *path1 && *path2; ++path1, ++path2)
		if (*path1 != *path2)
			return false;
#endif

	return *path1 == 0 && *path2 == 0;
}

bool InputFile::openInputFile(const char* name)
{
	if (!(file = fopen (name, "r")))
		return false;
		
	fileName = name;
	segment = buffer;
	changes = NULL;

	return true;
}
