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

#ifndef _ConfigFile_h_
#define _ConfigFile_h_

#include "Lex.h"
#include "RefObject.h"
#include "JString.h"

static const int HASH_SIZE		= 101;
static const int CONFIG_trace	= 1;
static const int CONFIG_list	= 2;
static const int CONFIG_verbose	= 4;

START_NAMESPACE
class InputFile;
class Element;
class ConfObject;

class ConfigFile : public Lex, public RefObject
{
public:
	ConfigFile(int configFlags);

//protected:
	virtual ~ConfigFile();

public:
	InputFile* openConfigFile();
	
	void			parse();
	Element*		parseObject();
	Element*		parseAttribute();
	ConfObject*		findObject(const char* objectType, const char* objectName);
	const char*		getRootDirectory();
	ConfObject*		getObject(const char* objectType);
	Element*		findGlobalAttribute(const char *attributeName);
	const char*		getInstallDirectory();
	virtual JString	expand(JString rawString);

	const char* translate(const char *value, Element *object);
	void init(int configFlags);
	ConfigFile(const char* configFile, int configFlags);
	void wildCardInclude(const char* fileName);

	Element*	objects;

private:
	JString		rootDirectory;
	JString		installDirectory;
	JString		currentDirectory;
	Element*	hashTable [HASH_SIZE];

};
END_NAMESPACE

#endif

