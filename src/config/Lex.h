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

// Lex.h: interface for the Lex class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LEX_H__89737590_2C93_4F77_99AD_4C3881906C96__INCLUDED_)
#define AFX_LEX_H__89737590_2C93_4F77_99AD_4C3881906C96__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Stream.h"

START_NAMESPACE

static const int maxToken = 4096;

enum TokenType {
	END_OF_STREAM,
	PUNCT,
	NAME,
	QUOTED_NAME,
	NUMBER,
	END,
	QUOTED_STRING,
	SINGLE_QUOTED_STRING,
	DECIMAL_NUMBER,
	IP_ADDRESS,
	NONE
	};


static const int LEX_trace		= 1;
static const int LEX_list		= 2;
static const int LEX_verbose	= 4;
static const int LEX_upcase		= 8;

class InputStream;
class InputFile;
class Stream;

class Lex  
{
public:
	void captureStuff();
	bool getSegment();
	void pushStream (InputStream *stream);
	void setContinuationChar (char c);
	virtual void syntaxError (const char* expected);
	JString getName();
	JString reparseFilename();
	bool match (const char *word);
	bool isKeyword (const char *word);
	void setCommentString (const char *start, const char *end);
	void setLineComment (const char *string);
	void setCharacters (int type, const char *characters);
	void getToken();
	bool match (const char *pattern, const char *string);
	void skipWhite();
	Lex(const char *punctuation, int debugFlags);
	virtual ~Lex();

	InputStream	*inputStream;
	InputStream	*tokenInputStream;
	InputStream	*priorInputStream;
	Stream		stuff;
	bool		eol;
	int			tokenOffset;
	int			flags;
	char		captureStart;
	char		captureEnd;
	char		token [maxToken];
	int			tokenType;
	int			lineNumber;
	int			tokenLineNumber;
	int			priorLineNumber;
	const char	*ptr;
	const char	*end;
	const char	*lineComment;
	const char	*commentStart;
	const char	*commentEnd;
	char		continuationChar;
	char		charTable [256];
};

END_NAMESPACE

#endif // !defined(AFX_LEX_H__89737590_2C93_4F77_99AD_4C3881906C96__INCLUDED_)
