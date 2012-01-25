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
#include <memory.h>
#include "firebird.h"
#include "../jrd/common.h"
#include "ConfObject.h"
#include "Element.h"
#include "AdminException.h"
#include "ConfigFile.h"
#include "PathName.h"

#ifdef _WIN32
#ifndef strcasecmp
#define strcasecmp		stricmp
#define strncasecmp		strnicmp
#endif
#endif

#define IS_DIGIT(c)		(c >= '0' && c <= '9')

struct BooleanName
	{
	const char	*string;
	bool		value;
	};

static const BooleanName booleanNames [] = 
	{
	{"yes",		true},
	{"true",	true},
	{"false",	false},
	{"no",		false},
	{NULL}
	};
	
ConfObject::ConfObject(ConfigFile *confFile)
{
	configFile = confFile;
	configFile->addRef();
	object = NULL;
	chain = NULL;
}

ConfObject::~ConfObject(void)
{
	configFile->release();
	
	if (chain)
		chain->release();
}

bool ConfObject::matches(Element *element, const char* type, const char* string)
{
	if (element->name != type)
		return false;
	
	Element *attribute = element->findAttribute (0);
	
	if (!attribute)
		return false;
	
	//const char *name = configFile->translate (attribute->name, attribute->name);	
	JString name = expand (attribute->name);
	numberStrings = 0;
	end = buffer + sizeof (buffer);
	next = buffer;

	if (!match (0, name, string))
		return false;
	
	object = element;
	source = string;
	
	return true;
}

void ConfObject::putString(int position, const char* string, int stringLength)
{
	strings [position ] = next;
	
	if (next + stringLength + 1 >= end)
		throw AdminException ("ConfObject: string overflow");
	
	memcpy (next, string, stringLength);
	next [stringLength] = 0;
	next += stringLength + 1;
	
	if (position >= numberStrings)
		numberStrings = position + 1;
}

bool ConfObject::match(int position, const char* pattern, const char* string)
{
	char c;
	const char *s = string;

	for (const char *p = pattern; (c = *p++); ++s)
		if (c == '*')
			{
			if (!*p)
				{
				putString (position, string, (int) strlen (string));
				return true;
				}
			for (; *s; ++s)
				if (match (position + 1,  pattern+1, s))
					{
					putString (position, string, (int) (s - string));
					return true;
					}
			return false;
			}
		else if (!*s)
			return false;
		else if (c != '%' && c != *s)
			{
#ifdef _WIN32
			if (UPPER (c) == UPPER (*s))
				continue;
			if ((c == '/' || c == '\\') && (*s == '/' || *s == '\\'))
				continue;
#endif
			return false;
			}

	if (c || *s)
		return false;
	
	putString (position, string, (int) strlen (string));
	
	return true;
}

const char* ConfObject::getValue (const char* option, const char *defaultValue)
{
	Element *element = findAttribute (option);
	
	if (!element)
		return defaultValue;
		
	tempValue = expand (getValue (element));
	
	return tempValue;
}


int ConfObject::getValue(const char* option, int defaultValue)
{
	Element *element = findAttribute (option);
	
	if (!element)
		return defaultValue;
		
	JString value = expand (getValue (element));
	int n = 0;
	
	for (const char *p = value; *p;)
		{
		char c = *p++;
		if (c >= '0' && c <= '9')
			n = n * 10 + c - '0';
		else
			throw AdminException ("expected numeric value for option \"%s\", got \"%s\"", option, (const char*) value);
		}
	
	return n;
}

bool ConfObject::getValue(const char* option, bool defaultValue)
{
	Element *element = findAttribute (option);
	
	if (!element)
		return defaultValue;
		
	JString value = expand (getValue (element));
	
	for (const BooleanName *name = booleanNames; name->string; ++name)
		if (strcasecmp (name->string, value) == 0)
			return name->value;
	
	throw AdminException ("expected boolean value for option \"%s\", got \"%s\"", option, (const char*) value);
}

JString ConfObject::expand(const char* rawValue)
{
	if (!rawValue)
		return "";

	char temp [1024];
	char *p = temp;
	bool changed = false;
	
	for (const char *s = rawValue; *s;)
		{
		char c = *s++;
		if (c == '$')
			{
			if (*s == '(')
				{
				++s;
				char name [256], *n = name;
				while (*s && (c = *s++) != ')')
					*n++ = c;
				*n = 0;
				const char *subst = configFile->translate (name, object);
				if (!subst)
					throw AdminException ("can't substitute for \"%s\"", name);
				changed = true;
				for (const char *t = subst; *t;)
					*p++ = *t++;					
				}
			else
				{
				int n = 0;
				while (IS_DIGIT (*s))
					n = n * 10 + *s++ - '0';
				if (n > numberStrings)
					throw AdminException ("substitution index exceeds available segments");
				for (const char *t = (n == 0) ? (const char*) source : strings [n - 1]; *t;)
					*p++ = *t++;
				}
			}
		else
			*p++ = c;
		}
	
	*p = 0;
	
	if (!changed)
		return temp;

	return PathName::expandFilename (temp);	
}

JString ConfObject::getValue(const char* attributeName)
{
	Element *attribute = findAttribute (attributeName);
	
	if (!attribute)
		return "";
		
	return expand (getValue (attribute));
}

const char* ConfObject::getValue(int instanceNumber, const char* attributeName)
{
	Element *attribute = findAttribute (attributeName);
	
	if (!attribute)
		return "";
		
	Element *val = attribute->findAttribute (instanceNumber);
	
	if (!val)
		return "";
		
	tempValue = expand (val->name);
	
	return tempValue;
}

const char* ConfObject::getConcatenatedValues(const char* attributeName)
{
	Element *attribute = findAttribute (attributeName);
	JString value;
	
	if (!attribute)
		return value;
			
	for (Element *att = attribute->attributes; att; att = att->sibling)
		{
		if (!value.IsEmpty())
			value += " ";
		value += att->name;
		}
	
	tempValue = value;
	
	return tempValue;
}

Element* ConfObject::findAttribute(const char* attributeName)
{
	if (object)
		{
		Element *element = object->findChild (attributeName);
		if (element)
			return element;
		}
	
	if (chain)
		return chain->findAttribute (attributeName);

	return configFile->findGlobalAttribute (attributeName);
}

ConfObject* ConfObject::getChain(void)
{
	return chain;
}

void ConfObject::setChain(ConfObject* obj)
{
	if (chain)
		chain->release();
	
	if (chain = obj)
		chain->addRef();
}

const char* ConfObject::getValue(Element* attribute)
{
	if (!attribute)
		return NULL;
	
	Element *value = attribute->findAttribute (0);
	
	if (!value)
		return NULL;
	
	return value->name;
}

const char* ConfObject::getName(void)
{
	if (!object)
		return NULL;

	Element *attribute = object->findAttribute (0);
	
	if (!attribute)
		return NULL;
		
	return attribute->name;
}

ConfObject* ConfObject::findObject(const char* objectType, const char* objectName)
{
	return configFile->findObject(objectType, objectName);
}
