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

// Element.h: interface for the Element class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ELEMENT_H__36AF5EE7_6842_4000_B3C5_DB8DECEC79B7__INCLUDED_)
#define AFX_ELEMENT_H__36AF5EE7_6842_4000_B3C5_DB8DECEC79B7__INCLUDED_

#if defined _MSC_VER  && _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER > 1000

#include "JString.h"

START_NAMESPACE

class Stream;
class InputStream;

class Element  
{
public:
	Element* findChild (const char *name, const char *attribute, const char *value);
	Element* addAttribute (JString name, int value);
	void addAttribute (JString name);
	void gen (int level, Stream *stream);
	void setSource (int line, InputStream *stream);
	void init(JString elementName);
	const char* getAttributeValue (const char *name, const char *defaultValue);
	const char* getAttributeValue (const char *name);
	const char* getAttributeName (int position);
	Element* findChildIgnoreCase (const char *name);
	Element* addChild (JString name);
	Element* addAttribute (JString name, JString value);
	void indent (int level, Stream *stream);
	void genXML (int level, Stream *stream);
	Element* findAttribute (int seq);
	Element* findAttribute (const char *name);
	Element* findChild (const char *name);
	void print (int level);
	void addAttribute (Element *child);
	void addChild (Element *child);
	Element (JString elementName, JString elementValue);
	Element(JString elementName);
	virtual ~Element();

	JString		name;
	JString		value;
	JString		innerText;
	JString		outerText;
	Element		*sibling;
	Element		*parent;
	Element		*children;
	Element		*attributes;
	int			lineNumber;
	int			numberLines;
	InputStream	*inputStream;
	static int analyseText(const char* text);
	void putQuotedText(const char* text, Stream* stream);
	static int analyzeData(int length, const UCHAR* data);
};

END_NAMESPACE

#endif // !defined(AFX_ELEMENT_H__36AF5EE7_6842_4000_B3C5_DB8DECEC79B7__INCLUDED_)
