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

#ifndef _DIR_LIST_H_
#define _DIR_LIST_H_

class ParsedPath {
	Firebird::string * PathElem;
	int nElem;
	Firebird::string SubPath(int n) const;
public:
	ParsedPath(void);
	ParsedPath(const Firebird::string & path);
	~ParsedPath();
	void Parse(const Firebird::string & path);
	Firebird::string operator[](int n);
	operator Firebird::string();
	bool operator==(const char * path);
	bool Contains(const ParsedPath & pPath);
};

class DirectoryList {
private:
	ParsedPath * ConfigDirs;
	int nDirs;
	void Initialize(void);
protected:
	// for various configuration parameters
	// returns parameter value
	virtual const Firebird::string GetConfigString(void) = 0;
public:
	DirectoryList();
	~DirectoryList();
	// Check, whether Path inside this DirectoryList
	bool IsPathInList(const Firebird::string & Path);
};
#endif //	_DIR_LIST_H_	
