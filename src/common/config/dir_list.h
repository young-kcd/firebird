/*
 *	PROGRAM:	Client/Server Common Code
 *	MODULE:		dir_list.h
 *	DESCRIPTION:	Directory listing config file operation
 *
 *  The contents of this file are subject to the Initial
 *  Developer's Public License Version 1.0 (the "License");
 *  you may not use this file except in compliance with the
 *  License. You may obtain a copy of the License at
 *  http://www.ibphoenix.com/main.nfs?a=ibphoenix&page=ibp_idpl.
 *
 *  Software distributed under the License is distributed AS IS,
 *  WITHOUT WARRANTY OF ANY KIND, either express or implied.
 *  See the License for the specific language governing rights
 *  and limitations under the License.
 *
 *  The Original Code was created by Alexander Peshkoff
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2003 Alexander Peshkoff <peshkoff@mail.ru>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 */

#ifndef DIR_LIST_H
#define DIR_LIST_H

#include "fb_types.h"
#include "fb_string.h"
#include "fb_vector.h"
#include "../common/config/config_file.h"

//
// This class is used internally by DirectoryList
// to store single path in format of pre-parsed
// elements of that path & perform basic operations
// with this path representation.
// Because of it's internal nature it has only
// internally required subset of possible operators.
//
class ParsedPath {
	Firebird::PathName * PathElem;
	int nElem;
	Firebird::PathName SubPath(int n) const;
public:
	ParsedPath(void);
	ParsedPath(const Firebird::PathName& path);
	~ParsedPath();
	// Take new path inside
	void Parse(const Firebird::PathName& path);
	// Convert internal representation to traditional one
	operator Firebird::PathName() const;
	// Compare with path given by constant
	bool operator==(const char* path) const;
	// Check, whether pPath lies inside directory tree,
	// specified by *this ParsedPath. Also checks against
	// possible symbolic links.
	bool Contains(const ParsedPath& pPath) const;
};

class DirectoryList {
private:
	// ListMode must be changed together with ListKeys in dir_list.cpp
	enum ListMode {NotInitialized = -1, 
		None = 0, Restrict = 1, Full = 2, SimpleList = 3};
	ListMode Mode;
	ParsedPath * ConfigDirs;
	int nDirs;
	// Clear allocated memory and reinitialize
	void Clear(void) {
		delete[] ConfigDirs;
		ConfigDirs = 0;
		nDirs = 0;
		Mode = NotInitialized;
	}
	// Check, whether Value begins with Key, 
	// followed by any character from Next.
	// If Next is empty, Value shoult exactly match Key.
	// If Key found, sets Mode to KeyMode and returns true.
	bool KeyWord(const ListMode KeyMode, Firebird::PathName& Value, 
		Firebird::PathName Key, Firebird::PathName Next);
protected:
	// Used for various configuration parameters - 
	// returns parameter PathName from Config Manager.
	virtual const Firebird::PathName GetConfigString(void) const = 0;
	// Initialize loads data from Config Manager.
	// With simple mutex add-on may be easily used to 
	// load them dynamically. Now called locally
	// when IsPathInList() invoked first time.
	void Initialize(bool simple_mode = false);
	// May be used in derived classes for custom behaviour
	size_t DirCount() { return nDirs; }
	const ParsedPath* DirList() { return ConfigDirs; }
public:
	DirectoryList();
	virtual ~DirectoryList();

	// TODO
	// All functions, checking DirectoryList contents,
	// should be const. But with today's implementation
	// of Config manager, thay have to call Initialize.

	// Check, whether Path inside this DirectoryList
	bool IsPathInList(const Firebird::PathName & Path);
	// Search for file Name in all direcories of DirectoryList.
	// If found, return full path to it in Path. 
	// Otherwise Path = Name.
	// Last parameter defines required access rights
	// to the file - like access().
	void ExpandFileName(Firebird::PathName & Path, 
						const Firebird::PathName & Name,
						int Access);

	// Temporary - while we use STL basic_string 
	// for PathName representation we don't have this
	static void Trim(Firebird::PathName & s) {
		ConfigFile::stripLeadingWhiteSpace(s);
		ConfigFile::stripTrailingWhiteSpace(s);
	}
};

class TempDirectoryList : private DirectoryList {
public:
	// directory list item
	struct Item {
		Firebird::PathName dir;
		size_t size;
	};

	// public functions
	size_t Count() const;
	const Item& operator[](size_t) const;

	TempDirectoryList();

private:
	// implementation of the inherited function
	const Firebird::PathName GetConfigString() const;
	
	// private data
	Firebird::vector<Item> items;
};

#endif //	DIR_LIST_H
