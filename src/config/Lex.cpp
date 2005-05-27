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

// Lex.cpp: implementation of the Lex class.
//
//////////////////////////////////////////////////////////////////////

#include <string.h>
#include <stdio.h>
#include <memory.h>
#include "firebird.h"
#include "Lex.h"
#include "AdminException.h"
#include "InputStream.h"

#define WHITE_SPACE				" \t\n\r"
#define PUNCTUATION_CHARS		"<>="
#define MULTI_CHARS				"" //"+=*/%!~<>~^|&="
#define MAX_TOKEN				1024
#define UPCASE(c)				((c >= 'a' && c <= 'z') ? c - 'a' + 'A' : c)

#define WHITE					1
#define PUNCT					2
#define MULTI_CHAR				4
#define DIGIT					8
#define TERM					(WHITE | PUNCT)


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Lex::Lex(const char *punctuation, int debugFlags)
{
	lineComment = NULL;
	commentStart = NULL;
	memset (charTable, 0, sizeof (charTable));
	setCharacters (PUNCT, punctuation);
	setCharacters (WHITE, WHITE_SPACE);
	setCharacters (DIGIT, "0123456789");
	ptr = end = NULL;
	inputStream = NULL;
	tokenType = NONE;
	lineNumber = 0;
	continuationChar = 0;
	captureStart = captureEnd = 0;
	flags = debugFlags;
}

Lex::~Lex()
{
	if (inputStream)
		inputStream->release();
}

void Lex::skipWhite()
{
	for (;;)
		{
		while (ptr >= end)
			if (!getSegment())
				return;
		while (ptr < end)
			if (lineComment && lineComment [0] == *ptr && match (lineComment, ptr))
				{
				while (ptr < end && *ptr++ != '\n')
					;
				++inputStream->lineNumber;
				}
			else if (commentStart && commentStart [0] == *ptr && match (commentStart, ptr))
				{
				ptr += strlen (commentStart);
				while (ptr < end)
					if (commentEnd [0] == *ptr && match (commentEnd, ptr))
						{
						ptr += strlen (commentEnd);
						break;
						}
					else if (*ptr++ == '\n')
						++inputStream->lineNumber;
				}
			else if (*ptr == continuationChar && ptr [1] == '\n')
				{
				ptr += 2;
				++inputStream->lineNumber;
				}
			else if (charTable [*ptr] & WHITE)
				{
				if (*ptr++ == '\n')
					{
					eol = true;
					++inputStream->lineNumber;
					}
				}
			else
				return;
		}

}

bool Lex::match(const char *pattern, const char *string)
{
	while (*pattern && *string)
		if (*pattern++ != *string++)
			return false;

	return *pattern == 0;
}

void Lex::getToken()
{
	priorInputStream = tokenInputStream;
	priorLineNumber = tokenLineNumber;

	if (tokenType == END_OF_STREAM)
		throw AdminException ("expected token, got end-of-file");

	eol = false;
	skipWhite();

	if (tokenInputStream = inputStream)
		tokenLineNumber = inputStream->lineNumber;

	if (ptr >= end)
		{
		tokenType = END_OF_STREAM;
		strcpy (token, "-end-of-file-");
		return;
		}

	tokenOffset = inputStream->getOffset (ptr);
	char *p = token;
	char *endToken = token + sizeof (token);
	char c = *p++ = *ptr++;

	if (charTable [c] & PUNCT)
		tokenType = PUNCT;
	else if (c == '\'' || c == '"')
		{
		p = token;
		for (;;)
			{
			if (ptr >= end)
				{
				if (!getSegment())
					throw AdminException ("end of file in quoted string");
				}
			else if (*ptr == c)
				break;
			else
				{
				if (p >= endToken)
					throw AdminException ("token overflow in quoted string");
				*p++ = *ptr++;
				}
			}
		++ptr;
		tokenType = (c == '"') ? QUOTED_STRING : SINGLE_QUOTED_STRING;
		}
	else if (charTable [c] & DIGIT)
		{
		tokenType = NUMBER;
		while (ptr < end && (charTable [*ptr] & DIGIT))
			*p++ = *ptr++;
		}
	else
		{
		tokenType = NAME;
		if (flags & LEX_upcase)
			{
			p [-1] = UPCASE(c);
			while (ptr < end && !(charTable [*ptr] & (WHITE | PUNCT)))
				{
				c = *ptr++;
				*p++ = UPCASE(c);
				}
			}
		else
			while (ptr < end && !(charTable [*ptr] & (WHITE | PUNCT)))
				*p++ = *ptr++;
		}

	*p = 0;
}

void Lex::setCharacters(int type, const char *characters)
{
	for (const char *p = characters; *p; ++p)
		charTable [*p] |= type;
}

/***
void Lex::openFile(const char *fileName)
{
	inputStream = new InputFile (fileName, inputStream);
}
***/

void Lex::setLineComment(const char *string)
{
	lineComment = string;
}

void Lex::setCommentString(const char *start, const char *cend)
{
	commentStart = start;
	commentEnd = cend;
}


bool Lex::isKeyword(const char *word)
{
	return strcmp (token, word) == 0;
}

bool Lex::match(const char *word)
{
	if (!isKeyword (word))
		return false;

	if (*word == captureStart)
		captureStuff();

	getToken();

	return true;
}

JString Lex::reparseFilename()
{
	char *p = token;

	while (*p)
		++p;

	while (ptr < end && *ptr != '>' && !(charTable [*ptr] & WHITE))
		*p++ = *ptr++;

	*p = 0;
	JString string = token;
	//getToken();

	return string;
}

JString Lex::getName()
{
	if (tokenType != NAME)
		syntaxError ("name");

	JString name = token;
	getToken();
	
	return name;	
}

void Lex::syntaxError(const char *expected)
{
	AdminException exception ("expected %s, got \"%s\"", expected, token);

	if (tokenInputStream)
		exception.setLocation (tokenInputStream->getFileName(), tokenLineNumber);

	throw exception;
}

/***
InputFile* Lex::pushFile(const char *fileName)
{
	if (inputStream)
		inputStream->ptr = ptr;

	InputFile *inputFile = new InputFile (fileName, inputStream);
	inputStream = inputFile;
	ptr = end = NULL;
	tokenType = NONE;

	return inputFile;
}
***/

void Lex::setContinuationChar(char c)
{
	continuationChar = c;
}

void Lex::pushStream(InputStream *stream)
{
	stream->addRef();
	
	if (flags & LEX_trace)
		{
		const char *fileName = stream->getFileName();
		if (fileName)
			printf ("Opening %s\n", fileName);
		}

	if (inputStream)
		inputStream->ptr = ptr;

	stream->prior = inputStream;
	inputStream = stream;
	ptr = end = NULL;
	tokenType = NONE;
}

bool Lex::getSegment()
{
	if (!inputStream)
		{
		tokenType = END_OF_STREAM;
		eol = true;
		return false;
		}

	if (!(ptr = inputStream->getSegment()))
		{
		end = ptr;
		InputStream *prior = inputStream->prior;
		inputStream->close();
		inputStream->release();
		if (!(inputStream = prior))
			return false;
		ptr = inputStream->ptr;
		}

	end = (ptr) ? inputStream->getEnd() : NULL;

	if (end && (flags & LEX_list))
		printf ("    %s", ptr);
		
	return true;
}

void Lex::captureStuff()
{
	stuff.clear();

	for (;;)
		{
		if (ptr >= end)
			{
			if (!getSegment())
				return;
			continue;
			}
		if (*ptr == captureEnd)
			return;
		stuff.putCharacter (*ptr++);
		}
}
