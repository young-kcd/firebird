/*
 *	PROGRAM:	Client/Server Common Code
 *	MODULE:		dir_list.h
 *	DESCRIPTION:	Directory listing config file operation
 *
 * The contents of this file are subject to the Interbase Public
 * License Version 1.0 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy
 * of the License at http://www.Inprise.com/IPL.html
 *
 * Software distributed under the License is distributed on an
 * "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express
 * or implied. See the License for the specific language governing
 * rights and limitations under the License.
 *
 * Created by: Alex Peshkov <AlexPeshkov@users.sourceforge.net>
 *
 * All Rights Reserved.
 * Contributor(s): ______________________________________.
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
	Firebird::string * PathElem;
	int nElem;
	Firebird::string SubPath(int n) const;
public:
	ParsedPath(void);
	ParsedPath(const Firebird::string& path);
	~ParsedPath();
	// Take new path inside
	void Parse(const Firebird::string& path);
	// Convert internal representation to traditional one
	operator Firebird::string() const;
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
	bool KeyWord(const ListMode KeyMode, Firebird::string& Value, 
		Firebird::string Key, Firebird::string Next);
protected:
	// Used for various configuration parameters - 
	// returns parameter string from Config Manager.
	virtual const Firebird::string GetConfigString(void) const = 0;
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
	bool IsPathInList(const Firebird::string & Path);
	// Search for file Name in all direcories of DirectoryList.
	// If found, return full path to it in Path. 
	// Otherwise Path = Name.
	// Last parameter defines required access rights
	// to the file - like access().
	void ExpandFileName(Firebird::string & Path, 
						const Firebird::string & Name,
						int Access);

	// Temporary - while we use STL basic_string 
	// for string representation we don't have this
	static void Trim(Firebird::string & s) {
		ConfigFile::stripLeadingWhiteSpace(s);
		ConfigFile::stripTrailingWhiteSpace(s);
	}
};

class TempDirectoryList : private DirectoryList {
public:
	// directory list item
	struct Item {
		Firebird::string dir;
		size_t size;
	};

	// public functions
	size_t Count() const;
	const Item& operator[](size_t) const;

	TempDirectoryList();

private:
	// implementation of the inherited function
	const Firebird::string GetConfigString() const;
	
	// private data
	Firebird::vector<Item> items;
};

#endif //	DIR_LIST_H
