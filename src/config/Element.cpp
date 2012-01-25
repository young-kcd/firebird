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

// Element.cpp: implementation of the Element class.
//
//////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <string.h>
//#include <stdlib.h>
//#include "Engine.h"
#include "firebird.h"
#include "Element.h"
#include "Stream.h"
#include "InputStream.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#endif

static const int quoted = 1;
static const int illegal = 2;

static int charTable [256];
static int init();
static int foo = init();

int init()
{
	charTable['<'] = quoted;
	charTable['>'] = quoted;
	charTable['&'] = quoted;
	
	for (int n = 0; n < 10; ++n)
		charTable[n] = illegal;

	return 0;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Element::Element(JString elementName)
{
	init (elementName);
}

Element::Element(JString elementName, JString elementValue)
{
	init (elementName);
	value = elementValue;
}


void Element::init(JString elementName)
{
	name = elementName;
	attributes = NULL;
	children = NULL;
	parent = NULL;
	lineNumber = 0;
	numberLines = 0;
	inputStream = NULL;
}

Element::~Element()
{
	Element *child;

	while (child = children)
		{
		children = child->sibling;
		delete child;
		}

	while (child = attributes)
		{
		attributes = child->sibling;
		delete child;
		}

	if (inputStream)
		inputStream->release();
}


void Element::addChild(Element *child)
{
	child->parent = this;
	child->sibling = NULL;
	Element **ptr;

	for (ptr = &children; *ptr; ptr = &(*ptr)->sibling)
		;

	*ptr = child;
}

void Element::addAttribute(Element *child)
{
	child->parent = this;
	child->sibling = NULL;
	Element **ptr;

	for (ptr = &attributes; *ptr; ptr = &(*ptr)->sibling)
		;

	*ptr = child;
}

void Element::print(int level)
{
	printf ("%*s%s", level * 3, "", (const char*) name);
	Element *element;

	for (element = attributes; element; element = element->sibling)
		{
		printf (" %s", (const char*) element->name);
		if (element->value != "")
			printf ("=%s", (const char*) element->value);
		}

	printf ("\n");
	++level;
	for (element = children; element; element = element->sibling)
		element->print (level);
}

Element* Element::findChild(const char *childName)
{
	for (Element *child = children; child; child = child->sibling)
		if (child->name == childName)
			return child;

	return NULL;
}

Element* Element::findAttribute(const char *childName)
{
	for (Element *child = attributes; child; child = child->sibling)
		if (child->name == childName)
			return child;

	return NULL;
}

Element* Element::findAttribute(int seq)
{
	int n = 0;

	for (Element *attribute = attributes; attribute; attribute = attribute->sibling)
		if (n++ == seq)
			return attribute;

	return NULL;
}

void Element::genXML(int level, Stream *stream)
{
	indent (level, stream);
	stream->putCharacter ('<');
	stream->putSegment (name);

	for (Element *attribute = attributes; attribute; attribute = attribute->sibling)
		{
		stream->putCharacter (' ');
		stream->putSegment (attribute->name);
		stream->putSegment ("=\"");
		for (const char *p = attribute->value; *p; ++p)
			switch (*p)
				{
				case '"':	stream->putSegment ("&quot;"); break;
				case '\'':	stream->putSegment ("&apos;"); break;
				case '&':	stream->putSegment ("&amp;"); break;
				case '<':	stream->putSegment ("&lt;"); break;
				case '>':	stream->putSegment ("&gt;"); break;
				default:	stream->putCharacter (*p); break;
				}
		//stream->putSegment (attribute->value);
		stream->putCharacter ('"');
		}

	if (!innerText.IsEmpty())
		{
		stream->putCharacter('>');
		putQuotedText(innerText, stream);
		}
	else if (!children)
		{
		if (*name.getString() == '?')
			stream->putSegment ("?>\n");
		else
			stream->putSegment ("/>\n");
		return;
		}
	else
		stream->putSegment (">\n");
		
	++level;

	for (Element *child = children; child; child = child->sibling)
		child->genXML (level, stream);

	if (innerText.IsEmpty())
		indent (level - 1, stream);
		
	stream->putSegment ("</");
	stream->putSegment (name);
	stream->putSegment (">\n");
}

void Element::indent(int level, Stream *stream)
{
	int count = level * 3;

	for (int n = 0; n < count; ++n)
		stream->putCharacter (' ');
}

Element* Element::addAttribute(JString attributeName, JString attributeValue)
{
	Element *attribute = new Element (attributeName, attributeValue);
	addAttribute (attribute);

	return attribute;
}

Element* Element::addChild(JString childName)
{
	Element *element = new Element (childName);
	addChild (element);

	return element;
}

Element* Element::findChildIgnoreCase(const char *childName)
{
	for (Element *child = children; child; child = child->sibling)
		if (child->name.equalsNoCase (childName))
			return child;

	return NULL;
}

const char* Element::getAttributeName(int position)
{
	Element *element = findAttribute (position);

	if (!element)
		return NULL;

	return element->name;
}

const char* Element::getAttributeValue(const char *attributeName)
{
	return getAttributeValue (attributeName, NULL);
}

const char* Element::getAttributeValue(const char *attributeName, const char *defaultValue)
{
	Element *attribute = findAttribute (attributeName);

	if (!attribute)
		return defaultValue;

	return attribute->value;
}

void Element::setSource(int line, InputStream *stream)
{
	lineNumber = line;
	inputStream = stream;
	inputStream->addRef();
}

void Element::gen(int level, Stream *stream)
{
	for (int n = 0; n < level; ++n)
		stream->putSegment ("   ");

	if (children)
		stream->putCharacter ('<');

	stream->putSegment (name);
	Element *element;

	for (element = attributes; element; element = element->sibling)
		{
		stream->putCharacter (' ');
		stream->putSegment (element->name);
		if (element->value != "")
			{
			stream->putCharacter ('=');
			stream->putSegment (element->value);
			}
		}

	if (!children)
		{
		stream->putCharacter ('\n');
		return;
		}

	stream->putSegment (">\n");
	++level;

	for (element = children; element; element = element->sibling)
		element->gen (level, stream);

	stream->putSegment ("</");
	stream->putSegment (name);
	stream->putSegment (">\n");
}

void Element::addAttribute(JString attributeName)
{
	addAttribute (new Element (attributeName));
}

Element* Element::addAttribute(JString attributeName, int value)
{
	char buffer [32];
	sprintf (buffer, "%d", value);
	
	return addAttribute (attributeName, buffer);
}

Element* Element::findChild(const char *childName, const char *attribute, const char *attributeValue)
{
	for (Element *child = children; child; child = child->sibling)
		if (child->name == childName)
			{
			const char *p = child->getAttributeValue (attribute, NULL);
			if (p && strcmp (p, attributeValue) == 0)
				return child;
			}

	return NULL;
}

int Element::analyseText(const char* text)
{
	int count = 0;
	
	for (const char *p = text; *p; p++)
		{
		int n = charTable[(UCHAR) *p];
		
		if (n)
			{
			if (n & illegal)
				return -1;
			
			++count;
			}
		}
	
	return count;
}

void Element::putQuotedText(const char* text, Stream* stream)
{
	const char *start = text;
	const char *p;
	
	for (p = text; *p; p++)
		if (charTable[(UCHAR) *p])
			{
			const char *escape = NULL;
			if (*p == '>')
				escape = "&gt;";
			else if (*p == '<')
				escape = "&lt;";
			else if (*p == '&')
				escape = "&amp;";
			else
				continue;
				
			if (p > start)
				stream->putSegment(p - start, start, true);
		
			stream->putSegment(escape);
			start = p + 1;
			}
	
	if (p > start)
		stream->putSegment(p - start, start, true);		
}

int Element::analyzeData(int length, const UCHAR* bytes)
{
	int count = 0;
	
	for (const UCHAR *p = bytes; *p; p++)
		{
		int n = charTable[*p];
		
		if (n)
			{
			if (n & illegal)
				return -1;
			
			++count;
			}
		}
	
	return count;
}
